#include "Game.h"
#include "GameConstants.h"
#include "LevelManager.h"

Game::Game(int fieldWidth, int fieldHeight, RenderSystem &renderer, EventBus *events)
    : field(fieldWidth, fieldHeight),
      player(0, 0, GameConstants::PLAYER_INITIAL_HEALTH, fieldWidth, fieldHeight, GameConstants::DEFAULT_HAND_SIZE),
      renderSystem(renderer),
      eventBus(events),
      gameRunning(true),
      enemiesKilled(0),
      currentLevel(1) {
    std::srand(std::time(nullptr));
    LevelManager::initializeLevel(*this, currentLevel);
}

void Game::render() const {
    int scoreTarget = LevelManager::getScoreTarget(currentLevel);
    renderSystem.render(field, player, enemies, buildings, towers, traps, allies, enemiesKilled, scoreTarget,
                        currentLevel);
}

bool Game::isPlayerAlive() const {
    return player.isAlive();
}

bool Game::isGameRunning() const {
    return gameRunning;
}

void Game::stop() {
    gameRunning = false;
}
