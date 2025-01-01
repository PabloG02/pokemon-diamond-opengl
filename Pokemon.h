#pragma once

#include <string>

class Pokemon {
  public:
    Pokemon(const std::string &name, int hp, int attack, int defense);

    void takeDamage(int damage);
    bool isFainted() const;

  // TODO: Make private
  //private:
    std::string name;
    int hp;
    int attack;
    int defense;
};
