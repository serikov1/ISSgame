#ifndef ISS_GAME_GUN_H
#define ISS_GAME_GUN_H

#include "Weapon.h"

class Gun : public Weapon {
public:
    Gun(int amount, double speed, int damage, double amplitude,
        std::string weaponArmLink, std::string weaponHandleLink, std::string weaponTrunkLink);
};

#endif //ISS_GAME_GUN_H
