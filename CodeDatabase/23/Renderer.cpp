#include "Renderer.h"
#include "Constants.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Player.h"
#include <iomanip>
#include <iostream>

void ConsoleRenderer::printHorizontalBorder(int width) {
  std::cout << "+";
  for (int i = 0; i < width; ++i) {
    std::cout << "-";
  }
  std::cout << "+" << std::endl;
}

char ConsoleRenderer::getEntitySymbol(int x, int y, const GameField *field) {
  if (!field)
    return ' ';

  
  const Player *player = field->getPlayer();
  if (player && player->getX() == x && player->getY() == y) {
    return PLAYER_SYMBOL;
  }

  
  for (const auto &pair : field->getEnemies()) {
    const Enemy *enemy = pair.second;
    if (enemy && enemy->getX() == x && enemy->getY() == y) {
      return ENEMY_SYMBOL;
    }
  }

  
  for (const auto &pair : field->getBuildings()) {
    const EnemyBuilding *building = pair.second;
    if (building && building->getX() == x && building->getY() == y) {
      return BUILDING_SYMBOL;
    }
  }

  
  for (const auto &pair : field->getTowers()) {
    const EnemyTower *tower = pair.second;
    if (tower && tower->getX() == x && tower->getY() == y) {
      return TOWER_SYMBOL;
    }
  }

  
  if (field->hasTrapAt(x, y)) {
    return TRAP_SYMBOL;
  }

  return EMPTY_SYMBOL;
}

void ConsoleRenderer::render(const GameField *field) {
  if (!field)
    return;

  int width = field->getWidth();
  int height = field->getHeight();

  
  std::cout << "\n\n";

  
  printHorizontalBorder(width);

  
  for (int y = 0; y < height; ++y) {
    std::cout << "|";
    for (int x = 0; x < width; ++x) {
      std::cout << getEntitySymbol(x, y, field);
    }
    std::cout << "|" << std::endl;
  }

  
  printHorizontalBorder(width);

  
  const Player *player = field->getPlayer();
  if (player) {
    std::cout << "\nPlayer HP: " << player->getHp()
              << " | Damage: " << player->getBaseDamage() << " | Attack: "
              << (player->getAttackType() == AttackType::MELEE ? "Melee"
                                                               : "Ranged")
              << " | Score: " << player->getScore();

    if (player->getHand()) {
      std::cout << " | Spells: " << player->getHand()->size() << "/"
                << player->getHand()->getCapacity();
    }
    std::cout << std::endl;
  }
}

void ConsoleRenderer::displayMessage(const std::string &message) {
  std::cout << message << std::endl;
}
