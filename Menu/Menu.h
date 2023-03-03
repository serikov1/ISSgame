#ifndef ISS_GAME_MENU_H
#define ISS_GAME_MENU_H

#include <SFML/Graphics.hpp>
#include "../Settings/Settings.h"
#include "../Camera/Camera.h"
#include "../Button/Button.h"

class Menu {
public:
    Menu();

    void drawMenu(sf::RenderWindow& window, double elapsedTime);
    void settings(sf::RenderTarget& window);
    void about(sf::RenderTarget& window);

    bool isPaused();
    void setPause(bool pause = true);

    bool isSmooth();
    void setSmooth(bool smooth);
    bool isCollision();
    void setCollision(bool collision);
    bool isTextures();
    void setTextures(bool textures);

private:
    std::vector<Button> buttons;

    bool pause_ = true;

    bool settings_ = false;
    bool about_ = false;

    bool textures_ = true;
    bool smooth_ = false;
    bool collision_ = true;
    double lastTime_ = 0;

    bool pressed_ = false;
};

#endif //ISS_GAME_MENU_H
