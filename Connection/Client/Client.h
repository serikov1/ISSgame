#ifndef ISS_GAME_CLIENT_H
#define ISS_GAME_CLIENT_H

#include "../../World/World.h"
#include "../../Camera/Camera.h"
#include "../Socket/Socket.h"

class Client {
public:
    explicit Client(World& world);
    bool isWorking() const;
    bool isConnected() const;
    Camera* localPlayer();
    void connect(sf::IpAddress ip, sf::Uint16 port);
    void disconnect();
    void update();
    void shoot(const std::string& name, double damage, double distance);

private:
    World& world_;
    Camera* localPlayer_;
    Socket socket_;
    double lastBroadcast_;
    bool working_;

    std::map<sf::Uint16, std::shared_ptr<Player>> players_;

    bool process();
    bool timeout(sf::Uint16 id);
};

#endif //ISS_GAME_CLIENT_H
