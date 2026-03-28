#ifndef SPELL_H
#define SPELL_H

#include <vector>
#include <string>

class Player;
class Enemy;
class Field;

class Spell {
public:
    virtual ~Spell() = default;
    virtual void cast(Player& player, std::vector<Enemy>& enemies, Field& field) = 0;
    virtual std::string getName() const = 0;
};

#endif