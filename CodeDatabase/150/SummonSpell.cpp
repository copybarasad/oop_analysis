#include "SummonSpell.hpp"
#include "GameState.hpp"
#include "SpellCaster.hpp"
#include "Ally.hpp"
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>

SummonSpell::SummonSpell(int summonCount, int range, int manaCost)
    : range_(range), manaCost_(manaCost), type_(SpellType::Summon), 
      summonCount_(summonCount) {}

bool SummonSpell::cast(int targetX, int targetY, GameState& gameState, SpellCaster& caster) {
    int casterX = caster.getPositionX();
    int casterY = caster.getPositionY();
    
    if (!checkRange(casterX, casterY, targetX, targetY) || !gameState.field.isValidPosition(targetX, targetY)) {
        return false;
    }
    
    const auto& cell = gameState.field.getCell(targetX, targetY);
    if (!cell.isEmpty() || !cell.isPassable()) {
        return false;
    }
    
    int summoned = 0;
    std::vector<std::pair<int, int>> possiblePositions = {
        {targetX-1, targetY}, {targetX+1, targetY}, 
        {targetX, targetY-1}, {targetX, targetY+1}
    };
    
    for (const auto& pos : possiblePositions) {
        if (summoned >= summonCount_) break;
        
        if (gameState.field.isValidPosition(pos.first, pos.second)) {
            const auto& targetCell = gameState.field.getCell(pos.first, pos.second);
            if (targetCell.isEmpty() && targetCell.isPassable()) {
                Ally ally(pos.first, pos.second);
                
                gameState.allies.push_back(ally);
                gameState.field.getCell(pos.first, pos.second).setHasAlly(true);
                summoned++;
                
                if (summonCount_ == 1) break;
            }
        }
    }
    
    return summoned > 0;
}

std::unique_ptr<ISpell> SummonSpell::clone() const {
    return std::make_unique<SummonSpell>(*this);
}

bool SummonSpell::checkRange(int casterX, int casterY, int targetX, int targetY) const {
    int distance = std::abs(casterX - targetX) + std::abs(casterY - targetY);
    return distance <= range_;
}

void SummonSpell::enhanceSummon(int bonus) {
    summonCount_ += bonus;
}

void SummonSpell::applyEnhancements(int enhancementCount) {
    enhanceSummon(enhancementCount);
}

void SummonSpell::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&range_), sizeof(range_));
    os.write(reinterpret_cast<const char*>(&manaCost_), sizeof(manaCost_));
    os.write(reinterpret_cast<const char*>(&summonCount_), sizeof(summonCount_));
}

void SummonSpell::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&range_), sizeof(range_));
    is.read(reinterpret_cast<char*>(&manaCost_), sizeof(manaCost_));
    is.read(reinterpret_cast<char*>(&summonCount_), sizeof(summonCount_));
}