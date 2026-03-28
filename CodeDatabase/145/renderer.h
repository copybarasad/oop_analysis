#ifndef RENDERER_H
#define RENDERER_H


#include <vector>
#include <memory>
#include "ally.h"
#include "game_field.h"

class GameField;
class Player;
class Enemy;
class EnemyBuilding;
class EnemyTower;
class SpellManager;

class Renderer {
public:
    Renderer() = default;

    void displayField(const GameField& field, const Player& player,
                      const std::vector<Enemy>& enemies,
                      const std::vector<EnemyBuilding>& buildings,
                      const std::vector<EnemyTower>& towers,
                      const std::vector<Ally>& allies,
                      const std::vector<Trap>& traps) const;

    void displayPlayerStatus(const Player& player) const;
    void displaySpells(const SpellManager& spellManager) const;
};

#endif