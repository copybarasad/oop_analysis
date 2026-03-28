#pragma once
#include <string>

class Player;
class EnemyManager;
class GameField;

enum class SpellType { Heal, Fireball, Teleport, DirectDamage, AreaDamage, Trap };

class Spell {
public:
    virtual ~Spell() = default;

    virtual void Cast(Player& player, EnemyManager& enemies, GameField& field) = 0;

    virtual std::string GetName() const = 0;

    virtual SpellType GetType() const = 0;
};