#ifndef ISS_GAME_SERVER_H
#define ISS_GAME_SERVER_H

#include <memory>
#include "../../World/World.h"
#include "../Socket/Socket.h"
#include "../../Camera/Camera.h"

class Server {
public:
    explicit Server(World& world);
    bool isWorking() const;
    bool start(sf::Uint16 port);
    void stop();
    void update();
    void addSpawn(Point point);
    void clearSpawns();

private:
    World& world_;
    Socket socket_;
    double lastBroadcast_;
    bool working_;

    std::map<sf::Uint16, std::shared_ptr<Player>> players_;
    std::vector<Point> spawns_;

    bool process();
    bool timeout(sf::Uint16 id);
};

#endif //ISS_GAME_SERVER_H
