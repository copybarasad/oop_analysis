#include "TrapSpell.h"
#include "game/Game.h"
#include "game/Trap.h"
#include <iostream>
#include <string>

namespace Game {
TrapSpell::TrapSpell(int baseDamage) : baseDamage(baseDamage) {}

std::string TrapSpell::getName() const { return "Lay Trap"; }
std::string TrapSpell::getDescription() const { return "Places a hidden trap that damages the first enemy to step on it."; }
bool TrapSpell::requiresTarget() const { return true; }

bool TrapSpell::cast(Game& game, const Utils::Point& target, int enhancementStacks) {
    // getMap() возвращает const Map&, поэтому переменная должна быть const
    const Map& map = game.getMap();
    if (!map.isValidPosition(target) || !map.getCell(target).isPassable() || game.isCellOccupied(target, true)) {
        game.notify({EventType::GenericMessage, std::string("Cannot place a trap here. The cell is occupied or impassable.")});
        return false;
    }
    
    int currentDamage = baseDamage + (enhancementStacks * 10);
    
    auto trap = std::make_unique<Trap>(target, currentDamage);
    game.addTrap(std::move(trap)); // addTrap вызовет notify
    
    return true;
}

SpellType TrapSpell::getType() const {
    return SpellType::Trap;
}

void TrapSpell::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&baseDamage), sizeof(baseDamage));
}

void TrapSpell::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&baseDamage), sizeof(baseDamage));
}

}