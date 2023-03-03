#include "Button.h"

void Button::select() {
    if (!selected_ && !pressed_) {
        button_.setTexture(*pressedTexture_);
        selected_ = true;
        button_.scale({1.01f, 1.01f});
    }
}

void Button::unSelect() {
    if (selected_ && !pressed_) {
        button_.setTexture(*texture_);
        selected_ = false;
        button_.scale({1/1.01f, 1/1.01f });
    }
}

void Button::press() {
    if (!pressed_) {
        button_.setTexture(*pressedTexture_);
        pressed_ = true;
    }
    else {
        button_.setTexture(*texture_);
        pressed_ = false;
    }
}

