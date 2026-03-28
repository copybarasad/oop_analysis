#pragma once
#include <utility>


class Field;
class EnemyManager;
class BuildingManager;
class PlayerManager;
class AllyManager;
class TowerManager;
class Hand;
class Spell;

struct SpellContext {
    std::pair<int,int> playerPos;
    std::pair<int,int> target;

    Field& field;
    EnemyManager& enemies;
    BuildingManager& buildings;
    PlayerManager& player;
    AllyManager& allies;
    TowerManager& towerManager;
    Hand& hand;
    std::shared_ptr<Spell> spell;
};
