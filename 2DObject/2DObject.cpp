#include "2DObject.h"

#include <utility>
#include "../GraphicsLoader/GraphicsLoader.h"


Object::Object(Point position, std::vector<Point> points, double height, const std::string& texture, Point velocity):
        position_(position), points_(std::move(points)), height_(height),
        texture_(*GraphicsLoader::loadTexture(texture)), velocity_(velocity) {}

Object::Object(const Object &object): position_(object.position_), points_(object.points_), height_(object.height_),
        texture_(object.texture_), velocity_(object.velocity_), nameLink_(object.nameLink_){}


void Object::draw(sf::RenderTarget &window) {
    sf::ConvexShape polygon;
    polygon.setPointCount(nodes().size());
    int i = 0u;
    for(auto node : nodes()) polygon.setPoint(i++, sf::Vector2f((float)node.x * SCALE, (float)node.y * SCALE));
    polygon.setOutlineColor(OUTLINE_COLOR);
    polygon.setOutlineThickness(OUTLINE_THICKNESS);
    polygon.setPosition((float)getX() * SCALE, (float)getY() * SCALE);
    window.draw(polygon);
}

sf::Texture &Object::loadTexture() {
    return texture_;
}

int sign(double number) {
    return number >= 0 ? 1 : -1;
}

bool Object::segmentsCrossing(const std::pair<Point, Point> &segmentFirst, const std::pair<Point,
                              Point> &segmentSecond, Point &point) {
    Point vectorFirst = segmentFirst.second - segmentFirst.first;
    Point vectorSecond = segmentSecond.second - segmentSecond.first;

    double productFirst = Point::cross(vectorFirst, (segmentSecond.first - segmentFirst.first));
    double productSecond = Point::cross(vectorFirst, (segmentSecond.second - segmentFirst.first));

    if (sign(productFirst) == sign(productSecond) || (productFirst == 0) || (productSecond == 0)) return false;

    productFirst = Point::cross(vectorSecond, (segmentFirst.first - segmentSecond.first));
    productSecond = Point::cross(vectorSecond, (segmentFirst.second - segmentSecond.first));

    if (sign(productFirst) == sign(productSecond) || (productFirst == 0) || (productSecond == 0)) return false;

    point.x = segmentFirst.first.x + vectorFirst.x * std::abs(productFirst) / std::abs(productSecond - productFirst);
    point.y = segmentFirst.first.y + vectorFirst.y * std::abs(productFirst) / std::abs(productSecond - productFirst);

    return true;
}

bool Object::isCross(const std::pair<Point, Point> &ray, std::pair<Point, Point> &wall, Point &point, double &distance) {
    Point crossPoint = {0, 0};
    std::pair<Point, Point> segment = {position_ + points_.back(), position_ + points_.front()};

    bool success = false;
    for (size_t i = 0; i < points_.size() - 1; i++) {
        if (segmentsCrossing(ray, segment, crossPoint) && (point - ray.first).abs() > (crossPoint - ray.first).abs()) {
            success = true;
            point = crossPoint;
            wall = std::move(segment);
        }
        segment = { position_ + points_[i], position_ + points_[i + 1] };
    }
    if (segmentsCrossing(ray, segment, crossPoint) && (point - ray.first).abs() > (crossPoint - ray.first).abs()) {
        success = true;
        point = crossPoint;
        wall = std::move(segment);
    }

    if (success) distance = (wall.second - point).abs();

    return success;
}

double Object::getHeight() const {
    return height_;
}

double Object::getX() const {
    return position_.x;
}

double Object::getY() const {
    return position_.y;
}

Point Object::getPosition() const {
    return position_;
}

std::string Object::getName() const {
    return nameLink_;
}

void Object::setPosition(Point position) {
    position_ = position;
}

void Object::setNameLink(std::string nameLink) {
    nameLink_ = std::move(nameLink);
}

void Object::shift(Point vector) {
    position_ += vector;
}

std::vector<Point> &Object::nodes() {
    return points_;
}

void Object::setPoints(std::vector<Point> points) {
    points_ = std::move(points);
}
