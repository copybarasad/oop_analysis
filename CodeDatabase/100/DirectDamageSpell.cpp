#include "DirectDamageSpell.h"
#include "../game_main/GameState.h"
#include "../game_objects/Player.h"
#include "../game_objects/Character.h"
#include "../game_objects/EnemyBuilding.h"
#include "../interface/ConsoleRenderer.h"
#include <cmath>
#include <sstream>

DirectDamageSpell::DirectDamageSpell(int damage, int range) : damage(damage), range(range) { updateStats(); }

// Использовать заклинание
bool DirectDamageSpell::use(Player& player, GameState& state, int targetX, int targetY) {
    std::ostringstream ss;

    // Проходимся по всем сущностям, если совпали координаты с (targetX, targetY) -> наноси урон и возвращаем true, иначе false
    // Враги
    for (const auto& character : state.getAllCharacters()) {
        if (character == &player || !character->isAlive()) continue;
        if (character->getX() == targetX && character->getY() == targetY) {
            character->takeDamage(damage);
            player.addScore(10);
            if (!character->isAlive()) {
                ss << Colors::GREEN << "Enemy was killed by the Direct Damage Spell!\n" << Colors::RESET;
            } else {
                ss << Colors::ORANGE << "Enemy was damaged by the Direct Damage Spell! Enemy's health: " 
               << character->getHealth() << "\n" << Colors::RESET;
            }
            state.addMessage(ss.str());
            return true;
        }
    }

    // Здания
    for (const auto& building : state.getAllBuildings()) {
        if (!building->isAlive()) continue;
        if (building->getX() == targetX && building->getY() == targetY) {
            building->takeDamage(damage);
            player.addScore(10);
            if (!building->isAlive()) {
                ss << Colors::GREEN << "Building was destroyed by the Direct Damage Spell!\n" << Colors::RESET;
            } else {
                ss << Colors::ORANGE << "Building was damaged by the Direct Damage Spell! Building health: " 
               << building->getHealth() << "\n" << Colors::RESET;
            }
            state.addMessage(ss.str());
            return true;
        }
    }

    // Башни
    for (const auto& tower : state.getAllTowers()) {
        if (!tower->isAlive()) continue;
        if (tower->getX() == targetX && tower->getY() == targetY) {
            tower->takeDamage(damage);
            player.addScore(10);
            if (!tower->isAlive()) {
                ss << Colors::GREEN << "Tower was destroyed by the Direct Damage Spell!\n" << Colors::RESET;
            } else {
                ss << Colors::ORANGE << "Tower was damaged by the Direct Damage Spell! Tower health: " 
               << tower->getHealth() << "\n" << Colors::RESET;
            }
            state.addMessage(ss.str());
            return true;
        }
    }

    ss << Colors::DARK_RED << "Direct Damage Spell wasn't used: no enemy in cell" << Colors::RESET;
    state.addMessage(ss.str());
    std::cout << ss.str();
    return false;
}

// Использовать заклинание на игрока (нужен для башни)
bool DirectDamageSpell::useOnPlayer(Player& player, GameState& state) {
    if (!player.isAlive()) return false;

    player.takeDamage(damage);

    std::ostringstream ss;
    if (!player.isAlive()) {
        ss << Colors::GREEN << "Player was killed by Enemy's Tower Spell!" << Colors::RESET;
    } else {
        ss << Colors::ORANGE << "Player was damaged by Enemy's Tower Spell! Player Health: "
           << player.getHealth() << Colors::RESET;
    }
    state.addMessage(ss.str());
    return true;
}

// Методы для улучшения
void DirectDamageSpell::upgrade() {
    level++;
    updateStats();
}

void DirectDamageSpell::updateStats() {
    if (level > 1) range++; // +1 к радиусу за уровень
}

// Геттеры
std::string DirectDamageSpell::getName() const { return "Direct Damage"; }

char DirectDamageSpell::getSymbol() const { return 'D'; }

int DirectDamageSpell::getRange() const { return range; }

int DirectDamageSpell::getLevel() const { return level; }

int DirectDamageSpell::getDamage() const { return damage; }

