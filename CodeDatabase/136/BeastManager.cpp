#include "BeastManager.h"
#include <random>
#include <algorithm>
#include <iostream>

BeastManager::BeastManager(GameField& field, const Position& wandererPos, Wanderer& wanderer)
    : field(field), wandererPosition(wandererPos), wanderer(wanderer) {}

void BeastManager::moveBeasts() {
    if (!wanderer.getIsAlive()) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    for (auto& beastData : beasts) {
        if (!beastData.second.getIsActive()) continue;
        Position& beastPos = beastData.first;
        std::vector<Position> possibleMoves = {
            Position(beastPos.x + 1, beastPos.y),
            Position(beastPos.x - 1, beastPos.y),
            Position(beastPos.x, beastPos.y + 1),
            Position(beastPos.x, beastPos.y - 1)
        };
        
        std::shuffle(possibleMoves.begin(), possibleMoves.end(), gen);
        
        for (const auto& move : possibleMoves) {
            if (move.x == wandererPosition.x && move.y == wandererPosition.y) {
                if (wanderer.getIsAlive()) {
                    wanderer.takeDamage(beastData.second.getDamage());
                    std::cout << "Beast attacks you for " << beastData.second.getDamage() << " damage!\n";
                }
                break;
            }
            else if (field.isValidPosition(move.x, move.y) && field.isCellPassable(move.x, move.y)) {
                Cell& targetCell = field.getCell(move.x, move.y);
            
                if (targetCell.getHasTrap()) {
                    beastData.second.takeDamage(targetCell.getTrapDamage());
                    std::cout << "Beast triggered a trap for " << targetCell.getTrapDamage() << " damage!\n";
                    targetCell.triggerTrap();
                }
                
                if (field.moveEntity(beastPos.x, beastPos.y, move.x, move.y, false)) {
                    beastPos = move;
                    break;
                }
            }
        }
    }
}

void BeastManager::addBeast(const Position& pos, const Beast& beast) {
    beasts.emplace_back(pos, beast);
    field.getCell(pos.x, pos.y).setHasBeast(true);
}

void BeastManager::removeDeadBeasts() {
    for (auto it = beasts.begin(); it != beasts.end(); ) {
        if (!it->second.isAlive()) {
            field.getCell(it->first.x, it->first.y).setHasBeast(false);
            it = beasts.erase(it);
        } else {
            ++it;
        }
    }
}

Beast* BeastManager::getBeastAt(const Position& pos) {
    for (auto& beastData : beasts) {
        if (beastData.first.x == pos.x && beastData.first.y == pos.y) {
            return &beastData.second;
        }
    }
    return nullptr;
}

const std::vector<std::pair<Position, Beast>>& BeastManager::getBeasts() const {
    return beasts;
}

void BeastManager::clearBeasts() {
    for (auto& beastData : beasts) {
        field.getCell(beastData.first.x, beastData.first.y).setHasBeast(false);
    }
    beasts.clear();
}

int BeastManager::countDeadBeasts() const {
    int count = 0;
    for (const auto& beastData : beasts) {
        if (!beastData.second.isAlive()) {
            count++;
        }
    }
    return count;
}

BeastManager::~BeastManager() {
    clearBeasts();
}