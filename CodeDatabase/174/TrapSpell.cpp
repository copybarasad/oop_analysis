#include "TrapSpell.h"
#include "Constants.h"

TrapSpell::TrapSpell(int dmg) : trapDamage(dmg) {}

bool TrapSpell::cast(GameField& field, const Position& pos, Player& player, Game& /*game*/) {
    if (!field.isValidPosition(pos)) {
        std::cout << "Invalid trap placement position!" << std::endl;
        return false;
    }

    if (field.getCellType(pos.getX(), pos.getY()) != CellType::EMPTY) {
        std::cout << "Cannot place trap here! Somethin on this cell already" << std::endl;
        return false; 
    }

    int effectiveDamage = trapDamage + player.getHand().getBuffCount()*trapDamage; 
    field.setCellType(pos.getX(), pos.getY(), CellType::TRAP);
    std::cout << "Trap placed at (" << pos.getX() << "," << pos.getY() 
    << "). Damage: " << effectiveDamage << ". Watch your step :)" << std::endl;
    return true;
}

std::string TrapSpell::getName() const {
    return "Set Trap";
}