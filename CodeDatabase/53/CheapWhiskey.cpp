#include "CheapWhiskey.h"
#include "GameField.h"
#include "Player.h"
#include "Trap.h"
#include "Constants.h"
#include <iostream>

using namespace GameConstants;

CheapWhiskey::CheapWhiskey(int spell_level)
    : SpellBase("CheapWhiskey", spell_level, CHEAPWHISKEY_DAMAGE) {
    
    if(spell_level > BEGIN_LEVEL_CHEAPWHISKEY && spell_level <= MAX_SPELL_LEVEL) {
        placementRange = spell_level * CHEAPWHISKEY_PLACEMENT_RANGE_PER_LEVEL;
    } else {
        setLevel(BEGIN_LEVEL_CHEAPWHISKEY);
        placementRange = BEGIN_CHEAPWHISKEY_PLACEMENT_RANGE;
    }
}

bool CheapWhiskey::use(int x, int y, GameField& field, Player& player) {
    if (!field.isValidPosition(x, y)) {
        std::cout << "Неверные координаты для размещения ловушки!" << std::endl;
        return false;
    }
    
    if (!isValidPlacement(x, y, player)) {
        std::cout << "Координаты вне зоны размещения ловушки! Максимальная дальность: " << placementRange << std::endl;
        return false;
    }
    
    Type_cell cellType = field.getCell(x, y).getType();
    
    if (cellType != EMPTY) {
        std::cout << "Невозможно разместить ловушку на этой клетке!" << std::endl;
        return false;
    }
    
    int currentDamage = getDamage();
    field.arr_traps.push_back(std::make_unique<Trap>(x, y, currentDamage));
    field.setCellType(x, y, TRAP);
    
    std::cout << "Ловушка CheapWhiskey (уровень " << getLevel() << ") размещена на позиции (" 
              << x << ", " << y << ") с уроном " << currentDamage << std::endl;
    return true;
}

bool CheapWhiskey::isValidPlacement(int x, int y, const Player& player) const {
    int playerX = player.getX();
    int playerY = player.getY();
    
    return (std::abs(playerX - x) <= placementRange && std::abs(playerY - y) <= placementRange);
}

void CheapWhiskey::upgrade() {
    if (canUpgrade()) {
        setLevel(getLevel() + 1);
        placementRange = getLevel() * CHEAPWHISKEY_PLACEMENT_RANGE_PER_LEVEL;
    }
}

bool CheapWhiskey::canUpgrade() {
    return getLevel() < MAX_SPELL_LEVEL;
}

void CheapWhiskey::resetLevel() {
    setLevel(BEGIN_LEVEL_CHEAPWHISKEY);
    placementRange = BEGIN_CHEAPWHISKEY_PLACEMENT_RANGE;
}

