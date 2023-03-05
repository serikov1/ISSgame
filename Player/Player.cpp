#include "Player.h"

Player::Player(Point position, double verticalPosition, double height, double health, const std::string& textureLink)
    : Circle(COLLISION_DISTANCE, position, height, textureLink, PLAYER_CONVEX_NUMBER),
    verticalPosition_(verticalPosition), health_(height) {}

bool
Player::isCross(const std::pair<Point, Point> &ray, std::pair<Point, Point> &wall, Point &point, double &distance) {
    return Circle::isCross(ray, wall, point, distance);
}

double Player::getVerticalPosition() const {
    return verticalPosition_;
}

void Player::setVerticalPosition(double verticalPosition) {
    verticalPosition_ = verticalPosition;
}

double Player::getHealth() const {
    return health_;
}

bool Player::reduceHealth(double damage) {
    health_ -= damage;
    if(health_ > 100) health_ = 100;
    return health_ <= 0;
}

void Player::setFullHealth() {
    health_ = 100;
}

void Player::setHealth(double h) {
    health_ = h;
}

ObjectType Player::type() {
    return ObjectType::Player;
}

