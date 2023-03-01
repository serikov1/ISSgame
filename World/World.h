#ifndef ISS_GAME_WORLD_H
#define ISS_GAME_WORLD_H

#include "../2DObject/2DObject.h"

class World : virtual public Drawable {
public:
    World(double length, double width, const std::string &sky_texture = SKY_TEXTURE,
          const std::string &floor_texture = FLOOR_TEXTURE);

    void draw(sf::RenderTarget &window) override;

    sf::Sprite &getFloorSprite();

    const sf::Texture &getSkyTexture() const;

    const sf::Texture &getFloorTexture() const;

    const std::map<std::string, Object &> &getObjects() const;

    bool addObject(Object &object, const std::string& name);

    bool removeObject(const std::string &name);

    bool isExist(const std::string &name) const;

    Object &findObject(const std::string &name);

    const Object &findObject(const std::string &name) const;

    Object &operator[](const std::string &name);

    const Object &operator[](const std::string &name) const;

    double getWidth() const;

    double getLength() const;
    
private:
    mutable std::map<std::string, Object&> mapObjects_;

    double length_ = 0;
    double width_ = 0;

    sf::Texture& skyTexture_;
    sf::Texture& floorTexture_;
    sf::Sprite floorSprite_;
};

#endif //ISS_GAME_WORLD_H
