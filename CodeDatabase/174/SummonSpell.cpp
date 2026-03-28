#include "SummonSpell.h"
#include "Constants.h"
#include "Game.h"
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

SummonSpell::SummonSpell(int count) : count(count) {}

bool SummonSpell::cast(GameField& field, const Position& /*playerPos*/, Player& player, Game& game) {
    int effectiveCount = count + player.getHand().getBuffCount()*count; 

    Position playerPos = player.getPosition();
    std::vector<Position> spawnPositions;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            Position newPos(playerPos.getX() + dx, playerPos.getY() + dy);

            if (field.isValidPosition(newPos) && field.isEmptyAt(newPos.getX(), newPos.getY())) {
                spawnPositions.push_back(newPos);
            }
        }
    }

    if (spawnPositions.empty()) {
        std::cout << "No empty space near player to summon ally!" << std::endl;
        return false;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(spawnPositions.begin(), spawnPositions.end(), gen);

    int summoned = 0;
    for (const auto& pos : spawnPositions) {
        if (summoned >= effectiveCount) break;
        
        auto ally = std::make_unique<AllyUnit>(pos.getX(), pos.getY());
        game.addAlly(std::move(ally));
        field.setCellType(pos.getX(), pos.getY(), CellType::ALLY);
        summoned++;
    }

    std::cout << "Summoned " << summoned << " ally unit(s)!" << std::endl;
    return true; 
}

std::string SummonSpell::getName() const {
    return "Summon Ally";
}