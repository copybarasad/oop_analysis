#include "../../Headers/Containers/GameData.h"

GameData::GameData(FieldContainer::FieldData field, Player::PlayerData player, Hand::HandData hand,
        std::vector<Enemy::EnemyData>& enemies, std::vector<StraightTower::StraightTowerData>& towers) :
        _field{ field }, _player{ player }, _hand{ hand }, _enemies{ enemies }, _towers{ towers }
{
}