#ifndef ISS_GAME_WEAPON_H
#define ISS_GAME_WEAPON_H

#include "../Settings/Settings.h"
#include "../Drawable/Drawable.h"

class Weapon : virtual public Drawable {
public:
//    Weapon(const Weapon &weapon);
    Weapon(int amount, double speed, int damage, double amplitude,
           std::string weaponArmLink, std::string weaponHandleLink, std::string weaponTrunkLink);

    void draw(sf::RenderTarget &window) override;

    double speed() const;
    int damage() const;

    bool fire();

private:
    double speed_ = 1;
    int damage_ = 0;
    int amount_ = 0;

    double shiftFire_ = 0;
    double amplitude_ = 100;

    double fireTime_ = 0;
    double elapsedTime_ = 0;
    double shootTime_ = 0;

    sf::Sprite weaponArm_;
    sf::Sprite weaponHandle_;
    sf::Sprite weaponTrunk_;
    sf::Sprite fire_;
    sf::Sprite aim_;

    std::string weaponArmLink_;
    std::string weaponHandleLink_;
    std::string weaponTrunkLink_;

};

#endif //ISS_GAME_WEAPON_H
