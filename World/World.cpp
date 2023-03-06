#include "World.h"
#include "../GraphicsLoader/GraphicsLoader.h"

World::World(double length, double width, const std::string &skyTexture, const std::string &floorTexture):
    length_(length), width_(width), skyTexture_(*GraphicsLoader::loadTexture(skyTexture)),
    floorTexture_(*GraphicsLoader::loadTexture(floorTexture)) {
    floorSprite_.setTexture(getFloorTexture());
}

void World::draw(sf::RenderTarget &window) {
    sf::ConvexShape background;
    background.setPointCount(4);
    background.setPoint(0, sf::Vector2f(0, 0));
    background.setPoint(1, sf::Vector2f((float)SCALE_WINDOW * SCALE/100, 0));
    background.setPoint(2, sf::Vector2f((float)SCALE_WINDOW * SCALE/100, (float)SCALE_WINDOW * SCALE/100));
    background.setPoint(3, sf::Vector2f(0, (float)SCALE_WINDOW * SCALE/100));
    background.setOutlineColor(OUTLINE_BACK_COLOR);
    background.setFillColor(FILL_BACK_COLOR);
    background.setOutlineThickness(OUTLINE_BACK_THICKNESS);
    background.setPosition(0, 0);
    window.draw(background);

    for (auto object : mapObjects_) object.second.draw(window);
}

sf::Sprite &World::getFloorSprite() {
    return floorSprite_;
}

const sf::Texture &World::getSkyTexture() const {
    return skyTexture_;
}

const sf::Texture &World::getFloorTexture() const {
    return  floorTexture_;
}

const std::map<std::string, Object &> &World::getObjects() const {
    return mapObjects_;
}

bool World::addObject(Object &object, const std::string& name) {
    object.setNameLink(name);
    return mapObjects_.insert({name, object}).second;
}

bool World::removeObject(const std::string &name) {
    return mapObjects_.erase(name);
}

bool World::isExist(const std::string &name) const {
    return mapObjects_.count(name) != 0;
}

Object &World::findObject(const std::string &name) {
    return mapObjects_.at(name);
}

const Object &World::findObject(const std::string &name) const {
    return mapObjects_.at(name);
}

Object &World::operator[](const std::string &name) {
    return findObject(name);
}

const Object &World::operator[](const std::string &name) const {
    return findObject(name);
}

double World::getWidth() const {
    return width_;
}

double World::getLength() const {
    return length_;
}


