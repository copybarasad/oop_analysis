#include "TrapSpell.h"
#include "Game.h"
#include "Trap.h"
#include <cmath>
#include <string>

TrapSpell::TrapSpell(int radius, int trapDamage)
    : Spell(radius, trapDamage), enhancedDamage(trapDamage) {}

SpellResult TrapSpell::use(Game& game, int targetX, int targetY) {
    const Player& player = game.getPlayer();
    int playerX = player.getPositionX();
    int playerY = player.getPositionY();
    
    // Вычисляем расстояние до цели
    int dx = targetX - playerX;
    int dy = targetY - playerY;
    double distance = std::sqrt(dx * dx + dy * dy);
    
    if (distance > baseRadius) {
        return SpellResult(false, true, "Target is out of range!");
    }
    
    const GameField& field = game.getField();
    if (!field.isValidPosition(targetX, targetY)) {
        return SpellResult(false, true, "Invalid target position!");
    }
    
    const Cell& targetCell = field.getCell(targetX, targetY);
    // Проверяем, что клетка проходима (не стена)
    if (!targetCell.isPassable()) {
        return SpellResult(false, true, "Cannot place trap on wall!");
    }
    
    // Размещаем ловушку (можно ставить даже если есть союзник)
    if (game.placeTrap(targetX, targetY, enhancedDamage)) {
        return SpellResult(true, true, "Trap placed at (" + 
                          std::to_string(targetX) + ", " + std::to_string(targetY) + ")!");
    }
    
    return SpellResult(false, true, "Failed to place trap! Cell may be occupied by player, enemy, or building.");
}

std::string TrapSpell::getName() const {
    return "Trap";
}

std::string TrapSpell::getDescription() const {
    return "Places a trap that deals " + std::to_string(enhancedDamage) + 
           " damage when an enemy steps on it. Range: " + std::to_string(baseRadius) + " tiles.";
}

void TrapSpell::applyEnhancement(int radiusBonus, int damageBonus, int areaBonus, int summonBonus) {
    Spell::applyEnhancement(radiusBonus, damageBonus, areaBonus, summonBonus);
    enhancedDamage += damageBonus;
}

std::unique_ptr<Spell> TrapSpell::clone() const {
    return std::make_unique<TrapSpell>(*this);
}

