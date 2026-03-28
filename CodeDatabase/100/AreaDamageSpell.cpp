#include "AreaDamageSpell.h"
#include <sstream>
#include "../interface/ConsoleRenderer.h"
#include "../game_main/GameState.h"

AreaDamageSpell::AreaDamageSpell(int damage, int range, int areaSize)
    : damage(damage), range(range), areaSize(areaSize)  { updateStats(); }

// Испоьзовать заклинание
bool AreaDamageSpell::use(Player& player, GameState& state, int targetX, int targetY) {
    std::ostringstream ss;

    // Проход по всем клеткам и сущностям в радиусе [target; target + areaSize)
    for (int y = targetY; y < targetY + areaSize; y++) {
        for (int x = targetX; x < targetX + areaSize; x++) {
            // Враги
            for (const auto& character : state.getAllCharacters()) {
                if (character == &player || !character->isAlive()) continue;
                if (character->getX() == x && character->getY() == y) {
                    character->takeDamage(damage);
                    player.addScore(10);
                    if (!character->isAlive()) {
                        ss << Colors::GREEN << "Enemy was killed by the Area Damage Spell!" << std::endl << Colors::RESET;
                    } else {
                        ss << Colors::ORANGE << "Enemy was damaged by the Area Damage Spell! Enemy's health: " 
                        << character->getHealth() << std::endl << Colors::RESET;
                    }
                }
            }

            // Здания
            for (const auto& building : state.getAllBuildings()) {
                if (!building->isAlive()) continue;
                if (building->getX() == x && building->getY() == y) {
                    building->takeDamage(damage);
                    player.addScore(10);
                    if (!building->isAlive()) {
                        ss << Colors::GREEN << "Building was destroyed by the Area Damage Spell!" << std::endl << Colors::RESET;
                    } else {
                        ss << Colors::ORANGE << "Building was damaged by the Area Damage Spell! Building's health: " 
                        << building->getHealth() << std::endl << Colors::RESET;
                    }
                }
            }

            // Башни
            for (const auto& tower : state.getAllTowers()) {
                if (!tower->isAlive()) continue;
                if (tower->getX() == x && tower->getY() == y) {
                    tower->takeDamage(damage);
                    player.addScore(10);
                    if (!tower->isAlive()) {
                        ss << Colors::GREEN << "Tower was destroyed by the Area Damage Spell!" << std::endl << Colors::RESET;
                    } else {
                        ss << Colors::ORANGE << "Tower was damaged by the Area Damage Spell! Tower's health: " 
                        << tower->getHealth() << std::endl << Colors::RESET;
                    }
                }
            }
        }
    }

    state.addMessage(ss.str());
    return true;
}

// Методы для улучшения
void AreaDamageSpell::upgrade() {
    level++;
    updateStats();
}

void AreaDamageSpell::updateStats() {
    if (level > 1) areaSize++; // +1 к площади за уровень
}

// Геттеры
std::string AreaDamageSpell::getName() const { return "Area Damage"; }

char AreaDamageSpell::getSymbol() const { return 'A'; }

int AreaDamageSpell::getRange() const { return range; }

int AreaDamageSpell::getLevel() const { return level; }

int AreaDamageSpell::getDamage() const { return damage; }

int AreaDamageSpell::getAreaSize() const { return areaSize; }