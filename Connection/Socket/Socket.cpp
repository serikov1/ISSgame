#include <cmath>
#include "Socket.h"
#include "../../Time/Time.h"
#include "../../Settings/Settings.h"

// UDPConnection

UDPConnection::UDPConnection(sf::Uint16 id, sf::IpAddress ip, sf::Uint16 port)
    : id_(id), ip_(ip), port_(port), lastMsg(Time::time()) {}

bool UDPConnection::timeout() const {
    return Time::time() - lastMsg > TIMEOUT_SECONDS;
}

sf::Uint16 UDPConnection::getId() const {
    return id_;
}

sf::IpAddress UDPConnection::getIp() const {
    return ip_;
}

sf::Uint16 UDPConnection::getPort() const {
    return port_;
}

void UDPConnection::update() {
    lastMsg = Time::time();
}

bool UDPConnection::similar(sf::IpAddress ip, sf::Uint16 port) {
    return ip_ == ip && port_ == port;
}

void UDPConnection::send(sf::UdpSocket& socket, sf::Packet& packet) {
    socket.send(packet, ip_, port_);
}

// -------------------------------//-----------------------------------//

//ReliableMsg

ReliableMsg::ReliableMsg(sf::Packet &packet, sf::IpAddress address, sf::Uint16 port) :  packet_(packet), address_(address), port_(port), lastTry_(-INFINITY), firstTry_(Time::time()){}

bool ReliableMsg::trySend(sf::UdpSocket &socket) {
    if (Time::time() - firstTry_ > TIMEOUT_SECONDS) return false;
    if (Time::time() - lastTry_ > RELIABLE_RETRY_TIME) {
        lastTry_ = Time::time();
        socket.send(packet_, address_, port_);
    }
    return true;
}

// -------------------------------//-----------------------------------//

// MsgType

sf::Packet& operator<<(sf::Packet& packet, MsgType type) {
    return packet << (sf::Uint16)type;
}

sf::Packet& operator>>(sf::Packet& packet, MsgType& type) {
    sf::Uint16 value;
    packet >> value;
    type = (MsgType)value;
    return packet;
}

// -------------------------------//-----------------------------------//

// Socket

Socket::Socket() : serverId_(0), socketId_(0){
    socket_.setBlocking(false); // unblock socket (calls will always return immediately)
}

void Socket::addConnection(sf::Uint16 id, sf::IpAddress ip, sf::Uint16 port) {
    connections_.insert({id, UDPConnection(id, ip, port)});
}

void Socket::removeConnection(sf::Uint16 id) {
    connections_.erase(id);
}

bool Socket::bind(sf::Uint16 port) {
    bool result = socket_.bind(port) == sf::Socket::Status::Done;
    return result;
}

void Socket::send(sf::Packet &packet, sf::Uint16 id) {

}
void Socket::sendRely(sf::Packet &packet, sf::Uint16 id) {
    if (!connections_.count(id)) return;
    sf::Packet finalPacket;
    finalPacket << socketId_ << true << nextRelyMsgId_;
    finalPacket.append(packet.getData(), packet.getDataSize());
    relyPackets_.insert({ nextRelyMsgId_++, ReliableMsg(finalPacket, connections_.at(id).getIp(), connections_.at(id).getPort()) });
}

void Socket::unbind() {
    sf::Packet packet;
    packet << MsgType::Disconnect << socketId_;
    for (auto it = connections_.begin(); it != connections_.end();) {
        send(packet, it->first);
        connections_.erase(it++);
    }
    relyPackets_.clear();
    confirmTimes_.clear();
    socket_.unbind();
    setSocketId(0);
}

void Socket::setTimeoutCallback(std::function<bool(sf::Uint16)> callback) {
    timeoutCallback_ = std::move(callback);
}

void Socket::clearTimeoutCallback() {
    timeoutCallback_ = nullptr;
}

void Socket::setSocketId(sf::Uint16 id) {
    socketId_ = id;
}

sf::Uint16 Socket::getSocketId() const {
    return socketId_;
}

sf::Uint16 Socket::getServerId() const {
    return serverId_;
}

void Socket::update() {
    for (auto it = connections_.begin(); it != connections_.end();) {
        if (!it->second.timeout()) ++it;
        else {
            if (timeoutCallback_ && ! timeoutCallback_(it->first)) return;
            connections_.erase(it++);
        }
    }

    for (auto it = relyPackets_.begin(); it != relyPackets_.end();) {
        if (!it->second.trySend(socket_)) relyPackets_.erase(it++);
        else ++it;
    }

    for (auto it = confirmTimes_.begin(); it != confirmTimes_.end();) {
        if (Time::time() - it->second > TIMEOUT_SECONDS) confirmTimes_.erase(it++);
        else ++it;
    }
}

MsgType Socket::receive(sf::Packet &packet, sf::Uint16 &senderId) {
    sf::IpAddress ip;
    sf::Uint16 port;
    packet.clear();
    if (socket_.receive(packet, ip, port) != sf::Socket::Status::Done) return MsgType::None;

    // Read header
    bool reply = false;
    sf::Uint16 msgId = 0;
    MsgType type;
    senderId = 0;
    if (!(packet >> senderId >> reply >> msgId >> type)) return MsgType::Fake;


    if (connections_.count(senderId)) connections_.at(senderId).update();

    if (type == MsgType::Confirm) {
        relyPackets_.erase(msgId);
        return MsgType::Confirm;
    }

    if (type == MsgType::Connect) {
        sf::Uint32 version = 0;
        if (!(packet >> version) || version != NETWORK_VERSION) return MsgType::Fake;
        sf::Uint16 tmp;
        for (tmp = 64; tmp >= 1; tmp--) {
            if (!connections_.count(tmp)) senderId = tmp;
            else
            if (connections_.at(tmp).similar(ip, port)) break;
        }
        if (tmp == 0) connections_.insert({ senderId, UDPConnection(senderId, ip, port) });
        else return MsgType::Fake;
    }

    if (!connections_.count(senderId) || !connections_.at(senderId).similar(ip, port) || reply && confirmed(msgId, senderId))
        return MsgType::Fake;
    return type;
}

bool Socket::confirmed(sf::Uint16 msgId, sf::Uint16 senderId){
    sf::Packet confirmPacket;
    confirmPacket << socketId_ << false << msgId << MsgType::Confirm;
    connections_.at(senderId).send(socket_, confirmPacket);

    sf::Uint32 confirmId;
    confirmId = (senderId << 16) | msgId;

    bool repeat = confirmTimes_.count(confirmId);
    confirmTimes_[confirmId] = Time::time();

    return repeat;
}

