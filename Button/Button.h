#ifndef ISS_GAME_BUTTON_H
#define ISS_GAME_BUTTON_H

#include <SFML/Graphics.hpp>

struct Button {
    void select();
    void unSelect();
    void press();

    sf::Texture* texture_;
    sf::Texture* pressedTexture_;
    sf::Sprite button_;
    std::string name_;

    bool selected_ = false;
    bool pressed_ = false;
};

#endif //ISS_GAME_BUTTON_H
