#include "SummonSpell.h"
#include "game/Game.h"
#include "game/Ally.h"
#include <iostream>

namespace Game {
SummonSpell::SummonSpell(int baseSummons) : baseSummons(baseSummons) {}

std::string SummonSpell::getName() const { return "Summon Ally"; }
std::string SummonSpell::getDescription() const { return "Summons friendly allies to fight for you."; }
bool SummonSpell::requiresTarget() const { return false; }

bool SummonSpell::cast(Game& game, const Utils::Point& target, int enhancementStacks) {
    int numToSummon = baseSummons + enhancementStacks;
    
    auto freeCells = game.getFreeAdjacentPositions(game.getPlayer().getPosition());
    
    if (freeCells.empty()) {
        std::cout << "No space to summon allies!" << std::endl;
        return false;
    }
    
    int summonedCount = 0;
    for (size_t i = 0; i < numToSummon && i < freeCells.size(); ++i) {
        auto ally = std::make_unique<Ally>(freeCells[i]);
        std::cout << "Summoned an ally at (" << freeCells[i].row << ", " << freeCells[i].column << ")." << std::endl;
        game.addAlly(std::move(ally));
        summonedCount++;
    }
    
    return summonedCount > 0;
}

SpellType SummonSpell::getType() const {
    return SpellType::Summon;
}

void SummonSpell::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&baseSummons), sizeof(baseSummons));
}

void SummonSpell::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&baseSummons), sizeof(baseSummons));
}

}