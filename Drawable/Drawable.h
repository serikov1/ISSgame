#ifndef ISS_GAME_DRAWABLE_H
#define ISS_GAME_DRAWABLE_H

#include <SFML/Graphics.hpp>

class Drawable {
public:
    virtual void draw (sf::RenderTarget &window) = 0;
};

#endif //ISS_GAME_DRAWABLE_H
