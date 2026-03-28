#pragma once
#include "Hand.h"
#include "../Realizations/Enemy.h"
#include "../Realizations/Player.h"
#include "../Realizations/StraightTower.h"

struct GameData
{
    // Данные поля
    FieldContainer::FieldData _field;
    // Данные игрока
    Player::PlayerData _player;
    // Данные руки игрока
    Hand::HandData _hand;
    // Данные противника
    std::vector<Enemy::EnemyData> _enemies;
    // Данные башен
    std::vector<StraightTower::StraightTowerData> _towers;

    GameData(FieldContainer::FieldData field, Player::PlayerData player, Hand::HandData hand,
        std::vector<Enemy::EnemyData>& enemies, std::vector<StraightTower::StraightTowerData>& towers);
};
