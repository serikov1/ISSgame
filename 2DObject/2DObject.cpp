#include "2DObject.h"

Object::Object(Point position, std::vector<Point> points, double height, std::string texture, Point velocity):
        position_(position), points_(points), texture_(texture), {}

Object::Object(const Object &object) {

}

bool Object::segmentsCrossing(const std::pair<Point, Point> &segmentFirst, const std::pair<Point, Point> &segmentSecond,
                              Point &point) {
    return false;
}

bool Object::cross(const std::pair<Point, Point> &ray, std::pair<Point, Point> &wall, Point &point, double &distance) {
    return false;
}

void Object::draw(sf::RenderTarget &window) {

}

sf::Texture &Object::loadTexture() {
    return <#initializer#>;
}
