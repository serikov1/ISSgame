#include <chrono>
#include <cmath>
#include <utility>
#include "../2DPoint/2DPoint.h"
#include "Weapon.h"
#include "../GraphicsLoader/GraphicsLoader.h"

void Weapon::draw(sf::RenderTarget &window) {
    auto tp = std::chrono::system_clock::now();
    std::chrono::duration <double> elapsedTime = tp.time_since_epoch();
    double diff = elapsedTime.count() - elapsedTime_;
    elapsedTime_ = elapsedTime.count();


    if (shiftFire_ > 0) shiftFire_ -= 2 * amplitude_ * diff / speed_;
    else shiftFire_ = 0;
    double shift = 15 * (1 + std::cos(3 * elapsedTime_)); // Motion of the weapon

    Point weaponPosition = { SCREEN_WIDTH - weaponHandle_.getTextureRect().width + shift + shiftFire_, SCREEN_HEIGHT - weaponHandle_.getTextureRect().height + shift + shiftFire_ };

    // fire effect
    shootTime_ -= diff; // Time when you can't fire
    if (shootTime_ < 0) shootTime_= 0;

    fireTime_ -= diff;
    if (fireTime_ > 0) {
        fire_.setPosition(sf::Vector2f((float)(weaponPosition.x - 490), (float)(weaponPosition.y - 792)));
        window.draw(fire_);
    }

    // about weapon
    double armShift = 50 * std::sin(PI * 2 * shootTime_ / speed_);
    if (shootTime_ <= 0 || shootTime_ > speed_/2) armShift = 0;

    weaponArm_.setPosition(sf::Vector2f((float)(weaponPosition.x + armShift), (float)(weaponPosition.y + armShift))); // Shift arm when we shoot
    weaponHandle_.setPosition(sf::Vector2f((float)weaponPosition.x, (float)weaponPosition.y));
    weaponTrunk_.setPosition(sf::Vector2f((float)weaponPosition.x, (float)weaponPosition.y));

    window.draw(weaponTrunk_);
    window.draw(weaponHandle_);
    window.draw(weaponArm_);

    aim_.setPosition(sf::Vector2f((float)(SCREEN_WIDTH / 2 - aim_.getTextureRect().width / 50), (float)(SCREEN_HEIGHT / 2 - aim_.getTextureRect().height / 50)));
    window.draw(aim_);
}

double Weapon::speed() const {
    return speed_;
}

int Weapon::damage() const {
    return damage_;
}

bool Weapon::fire() {
    if ((amount_ > 0) && (shootTime_ == 0)) {
        shootTime_ = speed_;
        fireTime_ = .1;
        --amount_;
        shiftFire_ = amplitude_;
        return true;
    }
    return false;
}

Weapon::Weapon(int amount, double speed, int damage, double amplitude, const std::string &weaponArmLink, const std::string &weaponHandleLink,
               const std::string &weaponTrunkLink)
               : amount_(amount), speed_(speed), damage_(damage), amplitude_(amplitude), weaponArmLink_(weaponArmLink),
               weaponHandleLink_(weaponHandleLink), weaponTrunkLink_(weaponTrunkLink) {
    weaponArm_.setTexture(*GraphicsLoader::loadTexture(weaponArmLink_));
    weaponHandle_.setTexture(*GraphicsLoader::loadTexture(weaponHandleLink_));
    weaponTrunk_.setTexture(*GraphicsLoader::loadTexture(weaponTrunkLink_));

    aim_.setTexture(*GraphicsLoader::loadTexture(AIM_TEXTURE));
    aim_.scale(.05f, .05f);
    fire_.setTexture(*GraphicsLoader::loadTexture(FIRE_TEXTURE));

}

