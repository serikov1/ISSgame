#ifndef ISS_GAME_PLAYER_H
#define ISS_GAME_PLAYER_H

#include "../2DCircle/2DCircle.h"

class Player : public Circle {
public:
    explicit Player(Point position, double verticalPosition = 0, double height = 0.6, double health = 100, const std::string& texture = SKIN);

    // Check the intersection of the ray with player.
    // Returns true if ray intersects with player, false otherwise.
    // wall - hitted segment.
    // point - hit point.
    // distance - distance from the beginning of segment to the hit point.
    bool isCross(const std::pair<Point, Point>& ray, std::pair<Point, Point>& wall, Point& point, double& distance) override;

    double getVerticalPosition() const;
    void setVerticalPosition(double verticalPosition);

    double getHealth() const;
    bool reduceHealth(double damage);
    void setFullHealth();
    void setHealth(double h);

    ObjectType type() override;

private:
    double verticalPosition_;
    double health_;
};

#endif //ISS_GAME_PLAYER_H
