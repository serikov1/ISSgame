#ifndef ISS_GAME_SOCKET_H
#define ISS_GAME_SOCKET_H

#include <SFML/Network.hpp>

class UDPConnection {
public:
    UDPConnection(sf::Uint16 id, sf::IpAddress ip, sf::Uint16 port);
    bool timeout() const;
    void update();
    bool similar(sf::IpAddress ip, sf::Uint16 port) ;
    void send(sf::UdpSocket& socket, sf::Packet& packet);

    sf::Uint16 getId() const;
    sf::IpAddress getIp() const;
    sf::Uint16 getPort() const;
private:
    sf::Uint16 id_;
    sf::IpAddress ip_;
    sf::Uint16 port_;
    double lastMsg;
};

// -------------------------------//-----------------------------------//

class ReliableMsg {
public:
    ReliableMsg(sf::Packet& packet, sf::IpAddress address, sf::Uint16 port);
    bool trySend(sf::UdpSocket& socket);
private:
    sf::Packet packet_;
    sf::IpAddress address_;
    sf::Uint16 port_;
    double lastTry_;
    double firstTry_;
};

// -------------------------------//-----------------------------------//

enum class MsgType {
    None,         // empty message
    Fake,         // fake message
    Confirm,      // confirm receive
    Connect,      // connection to server
    Disconnect,   // disconnect from server
    WorldInit,    // world initialization from server to client
    WorldUpdate,  // world update from server to client
    PlayerUpdate, // player update from client to server
    Shoot,        // player shooted from client to server
};

sf::Packet& operator<<(sf::Packet& packet, MsgType type); // to write data into the packet.
sf::Packet& operator>>(sf::Packet& packet, MsgType& type); // to read data from the packet

// -------------------------------//-----------------------------------//

class Socket {
public:
    Socket();

    void addConnection(sf::Uint16 id, sf::IpAddress ip, sf::Uint16 port);
    void removeConnection(sf::Uint16 id);

    void send(sf::Packet& packet, sf::Uint16 id);
    void sendRely(sf::Packet& packet, sf::Uint16 id);
    bool bind(sf::Uint16 port); // bind the socket to a specific port
    void unbind();
    void setTimeoutCallback(std::function<bool(sf::Uint16)> callback);
    void clearTimeoutCallback();
    void update();

    void setSocketId(sf::Uint16 id);
    sf::Uint16 getSocketId() const;
    sf::Uint16 getServerId() const;

    MsgType receive(sf::Packet& packet, sf::Uint16& senderId);

private:
    sf::UdpSocket socket_;
    sf::Uint16 socketId_;
    sf::Uint16 serverId_;
    sf::Uint16 nextRelyMsgId_;

    std::map<sf::Uint16, UDPConnection> connections_;
    std::function<bool(sf::Uint16)> timeoutCallback_;

    std::map<sf::Uint16, ReliableMsg> relyPackets_;
    std::map<sf::Uint32, double> confirmTimes_;

    bool confirmed(sf::Uint16 msgId, sf::Uint16 senderId);
};

#endif //ISS_GAME_SOCKET_H
