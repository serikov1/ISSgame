#ifndef ISS_GAME_GRAPHICSLOADER_H
#define ISS_GAME_GRAPHICSLOADER_H
#include <SFML/Graphics.hpp>

namespace GraphicsLoader {
    // try to load texture from file
    // if success returns pointer to texture otherwise returns nullptr
    sf::Texture &loadTexture(const std::string &textureLink);
}
#endif //ISS_GAME_GRAPHICSLOADER_H
