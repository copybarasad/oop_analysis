#include "SummonSpell.h"
#include "Game.h"
#include "Ally.h"
#include <cmath>
#include <vector>
#include <string>

SummonSpell::SummonSpell(int radius, int summonCount)
    : Spell(radius, 0), enhancedSummonCount(summonCount) {}

SpellResult SummonSpell::use(Game& game, int targetX, int targetY) {
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
    
    // Ищем свободные клетки рядом с игроком для размещения союзников
    std::vector<std::pair<int, int>> spawnPositions;
    int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    
    for (int i = 0; i < 8 && spawnPositions.size() < enhancedSummonCount; ++i) {
        int newX = playerX + directions[i][0];
        int newY = playerY + directions[i][1];
        
        if (field.isValidPosition(newX, newY) && 
            field.canMoveTo(newX, newY) && 
            !field.getCell(newX, newY).hasPlayerOnCell()) {
            spawnPositions.push_back({newX, newY});
        }
    }
    
    if (spawnPositions.empty()) {
        return SpellResult(false, true, "No space to summon allies!");
    }
    
    // Призываем союзников
    int summoned = 0;
    for (size_t i = 0; i < spawnPositions.size() && i < static_cast<size_t>(enhancedSummonCount); ++i) {
        if (game.summonAlly(spawnPositions[i].first, spawnPositions[i].second)) {
            summoned++;
        }
    }
    
    if (summoned > 0) {
        return SpellResult(true, true, "Summoned " + std::to_string(summoned) + " ally(ies)!");
    }
    
    return SpellResult(false, true, "Failed to summon allies!");
}

std::string SummonSpell::getName() const {
    return "Summon";
}

std::string SummonSpell::getDescription() const {
    return "Summons " + std::to_string(enhancedSummonCount) + 
           " ally(ies) next to the player. Range: " + std::to_string(baseRadius) + " tiles.";
}

void SummonSpell::applyEnhancement(int radiusBonus, int damageBonus, int areaBonus, int summonBonus) {
    Spell::applyEnhancement(radiusBonus, damageBonus, areaBonus, summonBonus);
    enhancedSummonCount += summonBonus;
    if (enhancedSummonCount < 1) enhancedSummonCount = 1;
}

std::unique_ptr<Spell> SummonSpell::clone() const {
    return std::make_unique<SummonSpell>(*this);
}

