#include "TrapSpell.h"
#include "../game_main/GameState.h"
#include "../game_objects/Player.h"
#include "../interface/ConsoleRenderer.h"
#include <sstream>


TrapSpell::TrapSpell(int damage, int range) : damage(damage), range(range) { updateStats(); }

// Использовать заклинание
bool TrapSpell::use(Player& player, GameState& state, int targetX, int targetY) {

    const Cell& cell = state.getField().getCell(targetX, targetY);
    std::ostringstream ss;
    
    // Проверяем клетку, если клетка свободна, то ставим ловушку
    if (!cell.getIsPassable()) {
        ss << Colors::DARK_RED << "Can't place trap: cell isn't passable" << Colors::RESET;
        state.addMessage(ss.str());
        return false;
    }
    if (cell.getIsSlowing()) {
        ss << Colors::DARK_RED << "Can't place trap: cell is slowing" << Colors::RESET;
        state.addMessage(ss.str());
        return false;
    }
    if (cell.getHasTrap()) {
        ss << Colors::DARK_RED << "Can't place trap: there is already a trap here" << Colors::RESET;
        state.addMessage(ss.str());
        return false;
    }
    for (const auto& character : state.getAllCharacters()) {
        if (character->getX() == targetX && character->getY() == targetY) {
            if (character->isAlive()) {
                ss << Colors::DARK_RED << "Can't place trap: cell is already occupied." << Colors::RESET;
            } 
            state.addMessage(ss.str());
            return false;
        }
    }
    for (const auto& building : state.getAllBuildings()) {
        if (building->getX() == targetX && building->getY() == targetY) {
            if (building->isAlive()) {
                ss << Colors::DARK_RED << "Can't place trap: cell is already occupied." << Colors::RESET;
            } 
            state.addMessage(ss.str());
            return false;
        }
    }
    for (const auto& tower : state.getAllTowers()) {
        if (tower->getX() == targetX && tower->getY() == targetY) {
            if (tower->isAlive()) {
                ss << Colors::DARK_RED << "Can't place trap: cell is already occupied." << Colors::RESET;
            } 
            state.addMessage(ss.str());
            return false;
        }
    }
    for (const auto& ally : state.getAllAllies()) {
        if (ally->getX() == targetX && ally->getY() == targetY) {
            if (ally->isAlive()) {
                ss << Colors::DARK_RED << "Can't place trap: cell is already occupied." << Colors::RESET;
            } 
            state.addMessage(ss.str());
            return false;
        }
    }

    // Ставим ловушку
    state.setCellTrap(targetX, targetY, true);
    state.setLastTrapDamage(damage); // устанавливаем урон ловушки в GameState (нужен для улучшения чтобы изменять урон)
    ss << Colors::GREEN << "Trap placed at (" << targetX << ", " << targetY 
    << ") with damage " << damage << Colors::RESET;
    state.addMessage(ss.str());
    return true;

}

// Методы для улучшения
void TrapSpell::upgrade() {
    level++;
    updateStats();
}

void TrapSpell::updateStats() {
    if (level > 1) damage += 4; // +4 урона за уровень
}

// Геттеры
std::string TrapSpell::getName() const { return "Trap Spell"; }

char TrapSpell::getSymbol() const { return 'T'; }

int TrapSpell::getRange() const  { return range; }

int TrapSpell::getLevel() const { return level; }

int TrapSpell::getDamage() const { return damage; }