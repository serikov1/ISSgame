//
// Created by user on 05.03.2023.
//
#include <cmath>
#include "Server.h"
#include "../../Time/Time.h"

Server::Server(World &world) : world_(world), lastBroadcast_(-INFINITY), working_(false) {
    socket_.setTimeoutCallback(std::bind(&Server::timeout, this, std::placeholders::_1));
}

bool Server::isWorking() const {
    return working_;
}

bool Server::start(sf::Uint16 port) {
    return working_ = socket_.bind(port);
}

void Server::stop() {
    for (auto it = players_.begin(); it != players_.end();) {
        sf::Packet packet;
        packet << MsgType::Disconnect << it->first;
        socket_.send(packet, it->first);
        players_.erase(it++);
    }
    socket_.unbind();
    working_ = false;
}

void Server::update() {
    if (!working_) return;
    while (process());

    if (Time::time() - lastBroadcast_ > 1 / WORLD_UPDATE_RATE) {
        sf::Packet updatePacket;
        updatePacket << MsgType::WorldUpdate;

        for (auto&& player : players_) {
            Player& camera = *player.second;
            camera.reduceHealth(-1 * (Time::time() - lastBroadcast_));
            updatePacket << player.first << camera.getPosition().x << camera.getPosition().y << camera.getVerticalPosition() << camera.getHealth();
        }

        for (auto&& player : players_) {
            socket_.send(updatePacket, player.first);
        }

        lastBroadcast_ = Time::time();
    }
    socket_.update();
}

void Server::addSpawn(Point point) {
    spawns_.push_back(point);
}

void Server::clearSpawns() {
    spawns_.clear();
}

bool Server::process() {
    sf::Packet packet;
    sf::Uint16 senderId;
    MsgType type;

    if ((type = socket_.receive(packet, senderId)) == MsgType::None) return false;

    sf::Packet sendPacket;
    sf::Packet extraPacket;
    sf::Uint16 targetId;
    double buf[3];

    switch (type) {
        case MsgType::Connect:
            extraPacket << MsgType::Connect << NETWORK_VERSION << senderId;
            sendPacket << MsgType::WorldInit << senderId;
            players_.insert({senderId, std::shared_ptr<Player>(new Player(spawns_[senderId % spawns_.size()]))});
            for (auto&& player : players_) {
                Player& camera = *player.second;
                sendPacket << player.first << camera.getX() << camera.getY() << camera.getVerticalPosition() << camera.getHealth();
                if (player.first != senderId) socket_.sendRely(extraPacket, player.first);
            }
            socket_.sendRely(sendPacket, senderId);

            break;

        case MsgType::Disconnect:
            sendPacket << MsgType::Disconnect << senderId;
            players_.erase(senderId);
            socket_.removeConnection(senderId);
            for (auto&& player : players_) {
                socket_.sendRely(sendPacket, player.first);
            }
            break;

        case MsgType::PlayerUpdate:
            packet >> buf[0] >> buf[1] >> buf[2];
            players_.at(senderId)->setPosition({ buf[0], buf[1] });
            players_.at(senderId)->setVerticalPosition(buf[2]);
            break;

        case MsgType::Shoot:
            packet >> targetId >> buf[0] >> buf[1];
            sendPacket << MsgType::Shoot;
            if (players_.at(targetId)->reduceHealth(buf[0] / buf[1])) {
                sendPacket << true << spawns_[targetId % spawns_.size()].x << spawns_[targetId % spawns_.size()].y;
                players_.at(targetId)->setHealth(100);
                players_.at(targetId)->setPosition(spawns_[targetId % spawns_.size()]);
            }
            else {
                double dir = 2 * PI * rand() / RAND_MAX;
                sendPacket << false << 0.05 * cos(dir) << 0.05 * sin(dir);
            }
            socket_.sendRely(sendPacket, targetId);
            break;
    }
    return true;
}

bool Server::timeout(sf::Uint16 id) {
    sf::Packet packet;
    packet << MsgType::Disconnect << id;
    players_.erase(id);
    for (auto&& player : players_) {
        socket_.sendRely(packet, player.first);
    }
    return true;
}

