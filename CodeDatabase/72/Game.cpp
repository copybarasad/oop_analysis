#include "Game.h"
#include <memory>
#include <iostream>


Game::Game(std::shared_ptr<Field> field,
           std::shared_ptr<Player> player,
           LevelManager& level_manager)
    : field(field),
      level_manager(level_manager),
      shop(level_manager.getCurrentLevel())
{
    initializeGame(field, player);
    createControllers();
}

void Game::initializeGame(std::shared_ptr<Field> field, std::shared_ptr<Player> player) {
    if (player == nullptr && field == nullptr) {
        auto hand = std::make_shared<Hand>(4);
        auto new_player = std::make_shared<Player>(hand, 30, 15, Position{0, 0}, 100);
        this->field = std::make_shared<Field>(level_manager.getCurrentLevel(), new_player);
    } else if (player != nullptr && field == nullptr) {
        this->field = std::make_shared<Field>(level_manager.getCurrentLevel(), player);
    } else if (player != nullptr) {
        field->setPlayer(player);
        this->field = field;
    }
}

void Game::createControllers() {
    movement = std::make_unique<MovementController>(field);
    combat = std::make_unique<CombatController>(field);
    spawner = std::make_unique<EnemySpawner>(field);
    spellController = std::make_unique<SpellController>(field);
}

std::shared_ptr<Player> Game::getPlayer() {
    return field ? field->getPlayer() : nullptr;
}

bool Game::isPlayerAlive() const {
    auto player = field ? field->getPlayer() : nullptr;
    return player && player->isAlive();
}

bool Game::switchFightMode() {
    auto player = field->getPlayer();
    player->switchFightMode();
    return true;
}

void Game::updateGameState() {
    field->incrementSteps();
    spawner->spawnEnemies();
    movement->moveEnemies();

    for (auto tower : field->getEnemyTowers()) {
        tower.attack(*field);
    }
}

void Game::advanceToNextLevel() {
    level_manager.setCurrentLevel(level_manager.getCurrentLevel() + 1);
    auto player = field->getPlayer();

    field = std::make_shared<Field>(level_manager.getCurrentLevel(), player);

    movement = std::make_unique<MovementController>(field);
    combat = std::make_unique<CombatController>(field);
    spawner = std::make_unique<EnemySpawner>(field);
    spellController = std::make_unique<SpellController>(field);

    shop = SpellShop(level_manager.getCurrentLevel());
    player->restoreHealth();
}

void Game::handleSkipStep() {
    auto player = field->getPlayer();
    player->setSkipStep(false);

    movement->moveEnemies();

    for (auto tower : field->getEnemyTowers()) {
        tower.attack(*field);
    }
}

bool Game::isLevelComplete() const {
    return field && field->getEnemies().empty() && field->getEnemiesBuild().empty();
}