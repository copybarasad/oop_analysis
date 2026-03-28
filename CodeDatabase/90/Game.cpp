#include "Game.h"
#include <iostream>

Game::Game(int fieldWidth, int fieldHeight)
    : field(fieldWidth, fieldHeight), player(15, 8, 100, Position(1, 1)),
      enemyManager(), buildingManager(), gameRunning(true) {
  Position spawn = findPassableSpawnNear(Position(1, 1));
  player.setPosition(spawn);
  enemyManager.add(
      std::make_unique<Enemy>(5, 50,
                              findPassableSpawnNear(Position(
                                  Random::getInt(1, field.getWidth() - 2),
                                  Random::getInt(1, field.getHeight() - 2)))));
  enemyManager.add(
      std::make_unique<Enemy>(8, 40,
                              findPassableSpawnNear(Position(
                                  Random::getInt(1, field.getWidth() - 2),
                                  Random::getInt(1, field.getHeight() - 2)))));
  enemyManager.add(
      std::make_unique<Enemy>(3, 30,
                              findPassableSpawnNear(Position(
                                  Random::getInt(1, field.getWidth() - 2),
                                  Random::getInt(1, field.getHeight() - 2)))));

  buildingManager.add(std::make_unique<EnemyBuilding>(
      80,
      Position(Random::getInt(1, field.getWidth() - 2),
               Random::getInt(1, field.getHeight() - 2)),
      7, &enemyManager));
  combatSystem = std::make_unique<CombatSystem>(player, enemyManager,
                                                buildingManager, field);
  renderer =
      std::make_unique<Renderer>(field, player, enemyManager, buildingManager);
  inputHandler = std::make_unique<InputHandler>(*this);
  player.getHand().addRandom();
  renderer->setTargetingStatePtr(&inputHandler->getTargetingState());
  renderer->setMessagePtr(&inputHandler->getStatusMessage());
}

void Game::run() {
  inputHandler->enableRawMode();
  while (!shouldEnd()) {
    renderer->render();
    char input = inputHandler->getChar();
    if (!inputHandler->handleInput(input)) {
      stopGame();
      break;
    }
    if (!inputHandler->isTargetingActive()) {
      update();
    }
  }
  inputHandler->disableRawMode();
  if (isVictory()) {
    renderer->printVictoryMessage();
  } else if (isGameOver()) {
    std::cout << "\n=== GAME OVER ===" << std::endl;
    if (!player.isAlive())
      std::cout << "Player died!" << std::endl;
  }
}

Position Game::findPassableSpawnNear(Position startPos) const {
  if (field.canMoveTo(startPos) && !isOccupied(startPos)) {
    return startPos;
  }
  const int maxRadius = 5;
  for (int r = 1; r <= maxRadius; ++r) {
    for (int dy = -r; dy <= r; ++dy) {
      for (int dx = -r; dx <= r; ++dx) {
        int x = startPos.getX() + dx;
        int y = startPos.getY() + dy;
        if (!field.isValidPosition(Position(x, y)))
          continue;
        if (field.canMoveTo(Position(x, y)) && !isOccupied(Position(x, y))) {
          return Position(x, y);
        }
      }
    }
  }
  return startPos;
}

void Game::update() {
  for (const auto &e : enemyManager.getAlive()) {
    e->setAdjacentToPlayer(e->distanceTo(player) == 1);
  }
  combatSystem->enemyAttack();
  updateEnemies();
  for (const auto &building : buildingManager.getAll()) {
    if (building->isAlive()) {
      building->update(field.getWidth(), field.getHeight(), player, field);
    }
  }
  int defeated = combatSystem->cleanup();
  if (defeated > 0) {
    totalEnemiesDefeated += defeated;
    if (totalEnemiesDefeated % 2 == 0 || defeated > 1) {
      player.getHand().addRandom();
    }
  }
}

bool Game::movePlayer(Position newPos) {
  if (!field.canMoveTo(newPos))
    return false;
  if (isOccupied(newPos, &player))
    return false;
  player.setPosition(newPos);
  if (field.getCell(newPos).getType() == CellType::slowing) {
    player.setSlowed(true);
  }
  return true;
}

void Game::updateEnemies() {
  for (const auto &enemy : enemyManager.getAlive()) {
    Position oldPos = enemy->getPosition();
    if (enemy->distanceTo(player) != 1) {
      if (enemy->isSlowed()) {
        enemy->setSlowed(false);
      } else {
        enemy->move(player, field.getWidth(), field.getHeight());
      }
      if (!field.canMoveTo(enemy->getPosition()) ||
          isOccupied(enemy->getPosition(), enemy)) {
        enemy->setPosition(oldPos);
      }
    }
    bool moved = enemy->getPosition() != oldPos;
    if (moved &&
        field.getCell(enemy->getPosition()).getType() == CellType::slowing) {
      enemy->setSlowed(true);
    }
  }
}

bool Game::isOccupied(const Position &pos, const Entity *ignore) const {
  if (&player != ignore && player.isAlive() && player.getPosition() == pos) {
    return true;
  }
  for (const auto &e : enemyManager.getAll()) {
    if (e.get() != ignore && e->isAlive() && e->getPosition() == pos) {
      return true;
    }
  }
  for (const auto &b : buildingManager.getAll()) {
    if (b.get() != ignore && b->isAlive() && b->getPosition() == pos) {
      return true;
    }
  }
  return false;
}

bool Game::isGameOver() const { return !gameRunning || !player.isAlive(); }

bool Game::isVictory() const {
  bool allEnemiesDead = true;
  for (const auto &enemy : enemyManager.getAll()) {
    if (enemy->isAlive()) {
      allEnemiesDead = false;
      break;
    }
  }
  bool allBuildingsDead = true;
  for (const auto &building : buildingManager.getAll()) {
    if (building->isAlive()) {
      allBuildingsDead = false;
      break;
    }
  }
  return allEnemiesDead && allBuildingsDead;
}

bool Game::shouldEnd() const { return isGameOver() || isVictory(); }

void Game::stopGame() { gameRunning = false; }