#include "GameState.h"
#include "Field.h"
#include "Player.h"

GameState::GameState() 
    : field_(std::make_shared<Field>(15, 15))
    , player_(std::make_shared<Player>())
    , turnCount_(0)
    , level_(1)
    , score_(0) {}

GameState::GameState(std::shared_ptr<Field> field, std::shared_ptr<Player> player, int level)
    : field_(field)
    , player_(player)
    , turnCount_(0)
    , level_(level)
    , score_(0) {}

bool GameState::isPlayerAlive() const {
    return player_ && player_->isAlive();
}

bool GameState::areEnemiesPresent() const {
    if (!field_) return false;
    auto enemies = field_->findEnemies();
    auto bases = field_->findBases();
    return !enemies.empty() || !bases.empty();
}