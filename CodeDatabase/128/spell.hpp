#pragma once
#include <string>
#include <vector>
#include "enums.hpp"
#include "spell_defaults.hpp"

class Game;
class Field;
class Player;
struct EnemyUnit;
class EnemyBuilding;

class Spell {
public:
    virtual ~Spell() = default;

    virtual bool UseSpell(Game& game, const SpellParams& params) = 0;
    virtual std::string getName() const noexcept = 0;
    virtual SpellType getType() const noexcept = 0;
};
