#include "Pokemon.h"

Pokemon::Pokemon(const std::string &name, int hp, int attack, int defense)
    : name{name}, hp{hp}, attack{attack}, defense{defense} {}

void Pokemon::takeDamage(int damage) {
    hp = std::max(hp - damage, 0);
}

bool Pokemon::isFainted() const {
    return hp <= 0;
}