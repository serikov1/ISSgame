#include "Gun.h"

Gun::Gun(int amount, double speed, int damage, double amplitude, std::string &weaponArmLink, std::string &weaponHandleLink,
         std::string &weaponTrunkLink) : Weapon(amount, speed, damage, amplitude, weaponArmLink,
                                           weaponHandleLink, weaponTrunkLink) {}

