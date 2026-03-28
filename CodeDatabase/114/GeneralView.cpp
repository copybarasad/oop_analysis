#include "GeneralView.h"
#include <iostream>
#include "ISpell.h"

void GeneralView::printField(GameField& field) const {
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            if (field.getPlayer().getX() == x && field.getPlayer().getY() == y) {
                std::cout << "P";
            } else {
                bool foundEnemy = false;
                for (const auto& enemy : field.getEnemies()) {
                    if (enemy.getX() == x && enemy.getY() == y) {
                        std::cout << "E";
                        foundEnemy = true;
                        break;
                    }
                }
                if (foundEnemy) continue;
                bool foundBuilding = false;
                for (const auto& building : field.getBuilding()) {
                    if (building.getX() == x && building.getY() == y) {
                        std::cout << "B";
                        foundBuilding = true;
                        break;
                    }
                }
                if (foundBuilding) continue;
                bool foundPet = false;
                for (const auto& pet : field.getPets()) {
                    if (pet.getX() == x && pet.getY() == y) {
                        std::cout << "A";
                        foundPet = true;
                        break;
                    }
                }
                if (foundPet) continue;
                const Cell& cell = field.getCells()[y][x];
                switch (cell.getType()) {
                    case CellType::EMPTY: std::cout << "."; break;
                    case CellType::WALL: std::cout << "#"; break;
                    case CellType::SLOW: std::cout << "~"; break;
                    case CellType::TRAP: std::cout << "^"; break;
                    default: std::cout << "?"; break;
                }
            }
        }
        std::cout << std::endl;
    }
}

void GeneralView::printPlayer(const Player& player) const {
    std::cout << "Игрок: (" << player.getX() << ", " << player.getY() << ") "
        << "HP: " << player.getHealth() << ", "
        << "Деньги: " << player.getMoney() << std::endl;

    const auto& spells = player.getHand().getSpells();
    if (spells.empty()) {
        std::cout << "Заклинания: (нет)" << std::endl;
    } else {
        std::cout << "Заклинания: ";
        bool first = true;
        for (const auto& sp : spells) {
            if (!first) std::cout << ", ";
            first = false;
            switch (sp) {
                case SpellType::DIRECT_DAMAGE: std::cout << "Прямой урон"; break;
                case SpellType::AREA_DAMAGE: std::cout << "Урон по площади"; break;
                case SpellType::TRAP: std::cout << "Ловушка"; break;
                case SpellType::IMPROVE: std::cout << "Улучшатор"; break;
                case SpellType::SUMMON: std::cout << "Призыв союзника"; break;
                default: std::cout << "Неизвестно"; break;
            }
        }
        std::cout << std::endl;
    }
}

void GeneralView::printEnemies(const std::vector<Enemy>& enemies) const {
    for (const auto& enemy : enemies) {
        std::cout << "Враг: (" << enemy.getX() << ", " << enemy.getY() << ") "
            << "HP: " << enemy.getHealth() << std::endl;
    }
}

void GeneralView::printBuildings(const std::vector<EnemyBuilding>& buildings) const {
    for (const auto& building : buildings) {
        std::cout << "Здание: (" << building.getX() << ", " << building.getY() << ")" << std::endl;
    }
}

void GeneralView::print(const std::string& message) const {
    std::cout << message << std::endl;
}



