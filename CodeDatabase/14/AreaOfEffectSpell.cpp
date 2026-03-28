#include "AreaOfEffectSpell.hpp"
#include "GameBoard.hpp"
#include "player.hpp"
#include <cmath>

bool AreaOfEffectSpell::cast(GameBoard& board, Player& player, int targetX, int targetY) {
    int dx = player.getX() - targetX;
    int dy = player.getY() - targetY;
    double distance = std::sqrt(dx * dx + dy * dy);

    if (distance > getRange()) {
        board.addLogMessage("Target area is out of range.");
        return false;
    }

    int area_size = AOE_SPELL_AREA_SIZE + (player.getEnhancementStacks() * AOE_SPELL_AREA_SIZE_BONUS);
    int damage = AOE_SPELL_DAMAGE;

    board.addLogMessage("Casting AoE spell on a " + std::to_string(area_size) + "x" + std::to_string(area_size) + " area!");

    board.damageArea(targetX, targetY, area_size, damage);

    player.clearEnhancementStacks();
    return true;
}

std::string AreaOfEffectSpell::getName() const { return "Area of Effect"; }
std::string AreaOfEffectSpell::getDescription() const { return "Deals " + std::to_string(AOE_SPELL_DAMAGE) + " dmg in a 2x2 area."; }
double AreaOfEffectSpell::getRange() const { return AOE_SPELL_RANGE; }