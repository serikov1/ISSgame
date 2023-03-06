#include "Gun.h"

#include <utility>

Gun::Gun(int amount, double speed, int damage, double amplitude, std::string weaponArmLink, std::string weaponHandleLink,
         std::string weaponTrunkLink) : Weapon(amount, speed, damage, amplitude, std::move(weaponArmLink),
                                           std::move(weaponHandleLink), std::move(weaponTrunkLink)) {}

