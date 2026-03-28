#include "EnhancementSpell.h"
#include "game/Game.h"
#include <iostream>

namespace Game {
std::string EnhancementSpell::getName() const { return "Enhance"; }
std::string EnhancementSpell::getDescription() const { return "Empowers your next spell. Can be stacked."; }
bool EnhancementSpell::requiresTarget() const { return false; }

bool EnhancementSpell::cast(Game& game, const Utils::Point& target, int enhancementStacks) {
    game.getPlayer().addEnhancementStack();
    std::cout << "Your next spell is enhanced! Current enhancement level: " 
              << game.getPlayer().getEnhancementStacks() << std::endl;
              
    return true;
}

SpellType EnhancementSpell::getType() const {
    return SpellType::Enhancement;
}

void EnhancementSpell::serialize(std::ostream& os) const {
    // У этого заклинания нет полей, так что функция пустая
}

void EnhancementSpell::deserialize(std::istream& is) {
    // У этого заклинания нет полей, так что функция пустая
}

}