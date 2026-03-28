#include "Renderer.h"

void Renderer::printVictoryMessage() const {
  std::cout << "\n=== VICTORY! ===" << std::endl;
  std::cout << "All enemies and buildings destroyed!" << std::endl;
  std::cout << "Congratulations!" << std::endl;
}

void Renderer::render() const {
  clearScreen();
  printStatus();
  printFieldBorder();
  const struct LocalTargeting {
    bool active;
    int radius;
    Position cursor;
    int areaW;
    int areaH;
  } *tgt = static_cast<const LocalTargeting *>(targetingStatePtr);

  for (int y = 0; y < field.getHeight(); ++y) {
    std::cout << "|";
    for (int x = 0; x < field.getWidth(); ++x) {
      const Cell &cell = field.getCell(Position(x, y));

      bool inBlueRadius = false;
      if (tgt && tgt->active) {
        int dx = x - player.getPosition().getX();
        int dy = y - player.getPosition().getY();
        inBlueRadius = (dx * dx + dy * dy) <= (tgt->radius * tgt->radius);
      }
      bool inRedArea = false;
      if (tgt && tgt->active) {
        int ax0 = tgt->cursor.getX();
        int ay0 = tgt->cursor.getY();
        int ax1 = ax0 + (tgt->areaW > 0 ? tgt->areaW - 1 : 0);
        int ay1 = ay0 + (tgt->areaH > 0 ? tgt->areaH - 1 : 0);
        inRedArea = (x >= ax0 && x <= ax1 && y >= ay0 && y <= ay1);
      }

      if (player.getPosition() == Position(x, y)) {
        if (inRedArea) {
          std::cout << "\033[31mP \033[0m";
        } else if (inBlueRadius) {
          std::cout << "\033[34mP \033[0m";
        } else {
          std::cout << "P ";
        }
        continue;
      }
      bool drawn = false;
      for (const auto &building : buildingManager.getAll()) {
        if (building->isAlive() && building->getPosition() == Position(x, y)) {
          if (inRedArea) {
            std::cout << "\033[31mB \033[0m";
          } else if (inBlueRadius) {
            std::cout << "\033[34mB \033[0m";
          } else {
            std::cout << "B ";
          }
          drawn = true;
          break;
        }
      }
      if (drawn) {
        continue;
      }
      for (const auto &enemy : enemyManager.getAll()) {
        if (enemy->isAlive() && enemy->getPosition() == Position(x, y)) {
          if (inRedArea) {
            std::cout << "\033[31mE \033[0m";
          } else if (inBlueRadius) {
            std::cout << "\033[34mE \033[0m";
          } else {
            std::cout << "E ";
          }
          drawn = true;
          break;
        }
      }
      if (drawn) {
        continue;
      }
      switch (cell.getType()) {
      case CellType::empty:
        if (inRedArea) {
          std::cout << "\033[31m. \033[0m";
        } else if (inBlueRadius) {
          std::cout << "\033[34m. \033[0m";
        } else {
          std::cout << ". ";
        }
        break;
      case CellType::impassable:
        if (inRedArea) {
          std::cout << "\033[31m# \033[0m";
        } else if (inBlueRadius) {
          std::cout << "\033[34m# \033[0m";
        } else {
          std::cout << "# ";
        }
        break;
      case CellType::slowing:
        if (inRedArea) {
          std::cout << "\033[31m~ \033[0m";
        } else if (inBlueRadius) {
          std::cout << "\033[34m~ \033[0m";
        } else {
          std::cout << "~ ";
        }
        break;
      }
    }
    std::cout << "|" << std::endl;
  }
  printFieldBorder();
  if (tgt && tgt->active) {
    printTargetingMessage();
  } else {
    printCommands();
  }
  printLegend();
  printCombatMode();
  printHand();
  printMessage();
}

void Renderer::clearScreen() const { system("clear"); }

void Renderer::printStatus() const {
  std::cout << "=== RPG GAME ===" << std::endl;
  std::cout << "Health: " << player.getHealth()
            << " | Mode: " << player.getCombatModeString()
            << " | Damage: " << player.getCurrentDamage();
  if (player.isSlowed()) {
    std::cout << " | SLOWED!";
  }
  std::cout << std::endl;
}

void Renderer::printFieldBorder() const {
  std::cout << std::string(field.getWidth() * 2 + 2, '=') << std::endl;
}

void Renderer::printTargetingMessage() const {
  std::cout << "Targeting: W/A/S/D move, SPACE(cast), Q(cancel)" << std::endl;
}

void Renderer::printCombatMode() const {
  std::cout << "Combat mode: " << player.getCombatModeString() << std::endl;
}

void Renderer::printLegend() const {
  std::cout << "Legend: P=Player, E=Enemy, B=Building, #=Wall, .=Empty, ~=Slow"
            << std::endl;
}

void Renderer::printCommands() const {
  std::cout << "Commands: W(up) S(down) A(left) D(right) C(switch mode) "
               "SPACE(attack) E(spell) Q(quit) 1(save game) 2(load from save)"
            << std::endl;
}

void Renderer::printHand() const {
  std::cout << "Hand: ";
  const auto &handSpells = player.getHand().getAll();
  if (handSpells.empty()) {
    std::cout << "(empty)";
  } else {
    bool first = true;
    for (const auto &sp : handSpells) {
      if (!sp)
        continue;
      if (!first)
        std::cout << ", ";
      std::cout << sp->name();
      first = false;
    }
  }
  std::cout << std::endl;
}

void Renderer::printMessage() const {
  if (messagePtr && !messagePtr->empty()) {
    std::cout << *messagePtr << std::endl;
  }
}
