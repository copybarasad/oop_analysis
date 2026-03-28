#include "TrapSpell.hpp"
#include "GameBoard.hpp"
#include "player.hpp"
#include "trap.hpp"
#include <cmath>

bool TrapSpell::cast(GameBoard& board, Player& player, int targetX, int targetY) {
    int dx = player.getX() - targetX;
    int dy = player.getY() - targetY;
    double distance = std::sqrt(dx * dx + dy * dy);

    if (distance > getRange()) {
        board.addLogMessage("Cannot place a trap so far away.");
        return false;
    }

    int trap_damage = TRAP_DAMAGE + (player.getEnhancementStacks() * TRAP_DAMAGE_BONUS);

    Trap* new_trap = new Trap(targetX, targetY, trap_damage);

    if (board.addTrap(new_trap)) {
        board.addLogMessage("You place a magical trap!");
        player.clearEnhancementStacks();
        return true;
    } else {
        board.addLogMessage("Cannot place a trap on a wall or occupied cell.");
        return false;
    }
}

std::string TrapSpell::getName() const { return "Trap"; }
std::string TrapSpell::getDescription() const { return "Places a trap that deals " + std::to_string(TRAP_DAMAGE) + " dmg."; }
double TrapSpell::getRange() const { return TRAP_SPELL_RANGE; }