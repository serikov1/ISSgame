
#include "Menu.h"
#include "../GraphicsLoader/GraphicsLoader.h"

Menu::Menu() {
    buttons.assign(7, {});

    buttons[0].name_ = "START GAME";
    buttons[1].name_ = "SETTINGS";
    buttons[2].name_ = "ABOUT";
    buttons[3].name_ = "QUIT";
    buttons[0].texture_ = GraphicsLoader::loadTexture(PLAYGAME_TEXTURE);
    buttons[1].texture_ = GraphicsLoader::loadTexture(SETTINGS_TEXTURE);
    buttons[2].texture_ = GraphicsLoader::loadTexture(ABOUT_TEXTURE);
    buttons[3].texture_ = GraphicsLoader::loadTexture(QUIT_TEXTURE);

    buttons[0].pressedTexture_ = GraphicsLoader::loadTexture(PLAYGAME_PRESSED_TEXTURE);
    buttons[1].pressedTexture_ = GraphicsLoader::loadTexture(SETTINGS_PRESSED_TEXTURE);
    buttons[2].pressedTexture_ = GraphicsLoader::loadTexture(ABOUT_PRESSED_TEXTURE);
    buttons[3].pressedTexture_ = GraphicsLoader::loadTexture(QUIT_PRESSED_TEXTURE);

    buttons[4].name_ = "TEXTURING";
    buttons[5].name_ = "SMOOTHING";
    buttons[6].name_ = "COLLISION";
    buttons[4].texture_ = GraphicsLoader::loadTexture(TEXTURING_SELECT);
    buttons[5].texture_ = GraphicsLoader::loadTexture(SMOOTHING_SELECT);
    buttons[6].texture_ = GraphicsLoader::loadTexture(COLLISION_SELECT);

    buttons[4].pressedTexture_ = GraphicsLoader::loadTexture(TEXTURING_UNSELECT);
    buttons[5].pressedTexture_ = GraphicsLoader::loadTexture(SMOOTHING_UNSELECT);
    buttons[6].pressedTexture_ = GraphicsLoader::loadTexture(COLLISION_UNSELECT);

    for (size_t i = 0; i < buttons.size(); i++) {
        buttons[i].button_.setTexture(*buttons[i].texture_);
        if (i < 4) buttons[i].button_.setPosition((float)SCREEN_WIDTH / 2 - 170, (float)50 + 150 * i);
        else buttons[i].button_.setPosition((float)SCREEN_WIDTH / 2 - 170, (float)50 + 150 * (i - 4));
    }

    if (textures_) buttons[4].press();
    if (smooth_) buttons[5].press();
    if (collision_) buttons[6].press();
}

void Menu::drawMenu(sf::RenderWindow &window, double elapsedTime) {
    sf::ConvexShape back;
    back.setPointCount(4);
    back.setPoint(0, sf::Vector2f(0, 0));
    back.setPoint(1, sf::Vector2f(SCREEN_WIDTH, 0));
    back.setPoint(2, sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    back.setPoint(3, sf::Vector2f(0, SCREEN_HEIGHT));
    back.setPosition({ 0, 0 });
    back.setFillColor({ 255, 255, 255 });
    window.draw(back);

    float mouseX = (float)sf::Mouse::getPosition(window).x * window.getView().getSize().x / window.getSize().x;
    float mouseY = (float)sf::Mouse::getPosition(window).y * window.getView().getSize().y / window.getSize().y;

    bool isPressing = sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed_;
    pressed_ = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    for (auto i = 0u; i < buttons.size(); i++) {
        if (!buttons[i].button_.getGlobalBounds().contains(mouseX, mouseY)) buttons[i].unSelect();
        else{
            buttons[i].select();
            if (window.hasFocus() && isPressing) {
                buttons[i].unSelect();
                if (!settings_) {
                    if (buttons[i].name_ == "START GAME") {
                        pause_ = false;
                        isPressing = false;
                    }
                    else if (buttons[i].name_ == "SETTINGS") {
                        settings_ = true;
                        isPressing = false;
                    }
                    else if (buttons[i].name_ == "ABOUT") {
                        about_ = true;
                        isPressing = false;
                    }
                    else if (buttons[i].name_ == "QUIT") {
                        window.close();
                        isPressing = false;
                    }
                }
                else {
                    if (buttons[i].name_ == "TEXTURING") {
                        buttons[i].press();
                        textures_ = buttons[i].pressed_;
                        isPressing = false;
                    }
                    else if (buttons[i].name_ == "SMOOTHING") {
                        buttons[i].press();
                        smooth_ = buttons[i].pressed_;
                        isPressing = false;
                    }
                    else if (buttons[i].name_ == "COLLISION") {
                        buttons[i].press();
                        collision_ = buttons[i].pressed_;
                        isPressing = false;
                    }
                }
            }
        }
        if (!settings_ && !about_ && i < 4) window.draw(buttons[i].button_);
    }
    settings(window);
    about(window);
}

void Menu::settings(sf::RenderTarget &window) {
    if (!settings_) return;
    for (auto i = 4u; i < buttons.size(); i++) window.draw(buttons[i].button_);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) settings_ = false;
}

void Menu::about(sf::RenderTarget &window) {
    if (!about_) return;
    sf::Sprite button;
    button.setTexture(*GraphicsLoader::loadTexture(ABOUT_INFO));
    button.scale((float)SCREEN_WIDTH / 1920, (float)SCREEN_WIDTH / 1920);
    window.draw(button);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) about_ = false;
}

bool Menu::isPaused() {
    return pause_;
}

void Menu::setPause(bool pause) {
    pause_ = pause;
}

bool Menu::isSmooth() {
    return smooth_;
}

void Menu::setSmooth(bool smooth) {
    smooth_ = smooth;
}

bool Menu::isCollision() {
    return collision_;
}

void Menu::setCollision(bool collision) {
    collision_ = collision;
}

bool Menu::isTextures() {
    return textures_;
}

void Menu::setTextures(bool textures) {
    textures_ = textures;
}

