#ifndef CONSOLE_RENDERER_HPP
#define CONSOLE_RENDERER_HPP

#include <iostream>
#include <string>
#include <vector>

class GameField;
class Player;
class Enemy;

class ConsoleRenderer {
public:
  void renderMainMenu(bool hasSave) const {
    std::cout << "\n=== MAIN MENU ===" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Exit" << std::endl;
    if (hasSave) {
      std::cout << "(Save file detected)" << std::endl;
    }
    std::cout << "Choice: ";
  }

  void renderInvalidMenuChoice() const {
    std::cout << "Invalid choice! Please enter 1, 2 or 3." << std::endl;
  }

  void renderGameOver(bool won) const {
    std::cout << "\n=== "
              << (won ? "VICTORY! All levels completed!" : "GAME OVER")
              << " ===" << std::endl;
    std::cout << "1. Restart Game" << std::endl;
    std::cout << "2. Main Menu" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Choice: ";
  }

  void renderLevelHeader() const {
    std::cout << "Controls: w(up), s(down), a(left), d(right), c(switch combat "
                 "style), q(quit)"
              << std::endl;
    std::cout << "Spells: 1,2,3 + direction, m(show spells), b(buy spell)"
              << std::endl;
    std::cout << "Ranged attack: xw, xs, xa, xd" << std::endl;
  }

  void renderStatus(const Player &player) const;
  void renderEnemyInfo(const std::vector<Enemy> &enemies) const;
  void renderField(const GameField &field, const Player &player,
                   const std::vector<Enemy> &enemies) const;
};

#endif