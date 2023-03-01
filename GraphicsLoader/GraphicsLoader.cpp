#include <memory>
#include "GraphicsLoader.h"

namespace GraphicsLoader {
    namespace {
        // make map with internal access with the help empty namespace
        std::map<std::string, std::shared_ptr<sf::Texture>> textures;
    }

    sf::Texture *loadTexture(const std::string &textureLink) {
        // if texture is exist, ptr to it will be return
        auto iterator = textures.find(textureLink);
        if(iterator != textures.end()) return iterator->second.get();

        // try to load texture. if impossible, nullptr will be return
        std::shared_ptr<sf::Texture> newTexture(new sf::Texture);
        if(!newTexture->loadFromFile(textureLink)) return nullptr;

        // permission to copy texture if it less than need
        newTexture->setRepeated(true);
        // add new texture to map
        textures.emplace(textureLink, newTexture);

        // return ptr to new texture
        return newTexture.get();

    }

    void unloadTextures() {
    for (auto & texture : textures) texture.second.reset();
    textures.clear();
    }
}
