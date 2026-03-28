#include "game_framework.hpp"
#include "enemy.hpp"
#include "game.hpp"
#include "game_field.hpp"
#include "player.hpp"
#include <vector>

template <typename TInput>
bool GameController<TInput>::hasSaveFile(Game &game) {
  return game.hasSave();
}

template class GameController<ConsoleInputReader>;

void ConsoleRenderer::renderStatus(const Player &player) const {
  std::cout << "Health: " << player.get_hp() << " | ";
  std::cout << "Score: " << player.get_score() << " | ";
  std::cout << "Style: " << (player.getIsRangedCombat() ? "Ranged" : "Melee")
            << " | ";
  std::cout << "Damage: " << player.get_damage() << std::endl;
}

void ConsoleRenderer::renderEnemyInfo(const std::vector<Enemy> &enemies) const {
  std::cout << "Enemies: ";
  bool anyAlive = false;
  for (size_t i = 0; i < enemies.size(); ++i) {
    if (enemies[i].is_alive()) {
      std::cout << "E" << i + 1 << "(" << enemies[i].get_hp() << "HP) ";
      anyAlive = true;
    }
  }
  if (!anyAlive) {
    std::cout << "none";
  }
  std::cout << std::endl;
}

void ConsoleRenderer::renderField(const GameField &field, const Player &player,
                                  const std::vector<Enemy> &enemies) const {
  for (int y = 0; y < field.getHeight(); ++y) {
    for (int x = 0; x < field.getWidth(); ++x) {
      if (x == player.get_x() && y == player.get_y()) {
        std::cout << "P ";
      } else {
        bool enemyFound = false;
        for (const auto &enemy : enemies) {
          if (enemy.is_alive() && enemy.get_x() == x && enemy.get_y() == y) {
            std::cout << "E ";
            enemyFound = true;
            break;
          }
        }
        if (!enemyFound) {
          std::cout << (field.getField()[x][y].get_isPass() ? ". " : "# ");
        }
      }
    }
    std::cout << std::endl;
  }
}