#include <cmath>
#include <iostream>
#include "Client.h"
#include "../../Time/Time.h"

Client::Client(World &world) : world_(world), lastBroadcast_(-INFINITY), working_(false), localPlayer_(nullptr) {
    socket_.setTimeoutCallback(std::bind(&Client::timeout, this, std::placeholders::_1));
}

bool Client::isWorking() const {
    return working_;
}

bool Client::isConnected() const {
    return socket_.getSocketId();
}

Camera *Client::localPlayer() {
    return localPlayer_;
}

void Client::connect(sf::IpAddress ip, sf::Uint16 port) {
    sf::Packet packet;
    packet << MsgType::Connect << NETWORK_VERSION;
    socket_.bind(0);
    working_ = true;
    socket_.addConnection(socket_.getServerId(), ip, port);
    socket_.sendRely(packet, socket_.getServerId());
}

void Client::disconnect() {
    for (auto it = players_.begin(); it != players_.end();) {
        world_.removeObject(it->second->getName());
        players_.erase(it++);
    }
    localPlayer_ = nullptr;
    sf::Packet packet;
    packet << MsgType::Disconnect << socket_.getSocketId();
    socket_.send(packet, socket_.getServerId());
    socket_.unbind();
    working_ = false;
}

void Client::update() {
    while (working_ && process());
    if (!working_) return;

    if (Time::time() - lastBroadcast_ > (double)1 / WORLD_UPDATE_RATE && isConnected()) {
        sf::Packet updatePacket;
        updatePacket << MsgType::PlayerUpdate << localPlayer_->getX() << localPlayer_->getY() << localPlayer_->getVerticalPosition();
        socket_.send(updatePacket, socket_.getServerId());
        lastBroadcast_ = Time::time();
    }
    socket_.update();
}

void Client::shoot(const std::string &name, double damage, double distance) {
    if(name.empty()) return;
    sf::Packet packet;
    for (auto&& player : players_) {
        if (player.second->getName() == name) {
            packet << MsgType::Shoot << player.first << damage << distance;
            socket_.sendRely(packet, socket_.getServerId());
            break;
        }
    }
}

bool Client::process() {
    sf::Packet packet;
    sf::Uint16 senderId;
    MsgType type;

    if ((type = socket_.receive(packet, senderId)) == MsgType::None) return false;
    if (!isConnected() && type != MsgType::WorldInit) return true;

    sf::Packet sendPacket;
    sf::Packet extraPacket;
    sf::Uint16 targetId;
    bool revive;
    double buf[6];
    Player *player;

    switch (type)
    {
        case MsgType::Connect:
            std::cout<<"Connect";
            packet >> targetId;
            player = new Player({ 2.5, 0 });
            players_.insert({ targetId, std::shared_ptr<Player>(player)});
            world_.addObject(*players_.at(targetId), "Player" + std::to_string(targetId));
            localPlayer_->addPlayer(players_.at(targetId)->getName(), players_.at(targetId));
            break;

        case MsgType::Disconnect:
            packet >> targetId;
            if (targetId != socket_.getSocketId() && players_.count(targetId)) {
                world_.removeObject(players_.at(targetId)->getName());
                localPlayer_->removePlayer(players_.at(targetId)->getName());
                players_.erase(targetId);
            }
            else if (targetId == socket_.getSocketId()) {
                disconnect();
            }
            break;

        case MsgType::WorldInit:
            packet >> targetId;
            socket_.setSocketId(targetId);
            while (packet >> targetId >> buf[0] >> buf[1] >> buf[2] >> buf[3]) {
                if (targetId == socket_.getSocketId()) {
                    localPlayer_ = new Camera(world_, {2.5, 0});
                    player = localPlayer_;
                }
                else {
                    player = new Player({2.5, 0});
                }
                std::shared_ptr<Player> pplayer = static_cast<std::shared_ptr<Player>>(player);
                players_.insert({ targetId, pplayer});
                world_.addObject(*player, "Player" + std::to_string(targetId));
                player->setPosition({ buf[0], buf[1] });
                player->setVerticalPosition(buf[2]);
                player->setHealth(buf[3]);
            }
            for (auto&& p : players_) {
                if (p.first != socket_.getSocketId()) {
                    localPlayer_->addPlayer(p.second->getName(), p.second);
                }
            }
            break;

        case MsgType::WorldUpdate:
            while (packet >> targetId >> buf[0] >> buf[1] >> buf[2] >> buf[3]) {
                if (players_.count(targetId)) {
                    player = players_.at(targetId).get();
                    if (targetId != socket_.getSocketId()) {
                        player->setPosition({ buf[0], buf[1] });
                        player->setVerticalPosition(buf[2]);
                    }
                    player->setHealth(buf[3]);
                }
            }
            break;

        case MsgType::Shoot:
            packet >> revive >> buf[0] >> buf[1];
            if (revive)
                localPlayer_->setPosition({ buf[0], buf[1] });
            else
                localPlayer_->shiftPrecise({ buf[0], buf[1] });
            break;

        case MsgType::None:
            break;
        case MsgType::Fake:
            break;
        case MsgType::Confirm:
            break;
        case MsgType::PlayerUpdate:
            break;
    }
    return true;
}

bool Client::timeout(sf::Uint16 id) {
    if (id != socket_.getServerId()) return true;
    disconnect();
    return false;
}

