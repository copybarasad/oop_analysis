#include "Spell.h"
#include "GameField.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Trap.h"
#include "Ally.h"
#include "Player.h"
#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>

// ========== DIRECT DAMAGE SPELL ==========

DirectDamageSpell::DirectDamageSpell(int damage, int radius)
    : name("Direct Damage"), manaCost(15), enhancementLevel(0), damage(damage), radius(radius) {}

std::string DirectDamageSpell::getName() const { return name; }
int DirectDamageSpell::getManaCost() const { return manaCost; }

int DirectDamageSpell::getEnhancementLevel() const { return enhancementLevel; }
void DirectDamageSpell::setEnhancementLevel(int level) { enhancementLevel = level; }

bool DirectDamageSpell::cast(const Position& target, GameField& field, const Position& casterPos) {
    if (!field.isValidPosition(target)) {
        std::cout << "Invalid target position!\n";
        return false;
    }
    
    int distance = std::abs(target.x - casterPos.x) + std::abs(target.y - casterPos.y);
    int enhancedRadius = getEnhancedRadius();
    
    if (distance > enhancedRadius) {
        std::cout << "Target is too far! Maximum range: " << enhancedRadius << "\n";
        return false;
    }
    
    auto& cell = field.getCell(target);
    if (cell.hasObject()) {
        auto obj = cell.getObject();
        std::string type = obj->getType();
        
        if (type == "Enemy" || type == "EnemyBuilding" || type == "EnemyTower") {
            int enhancedDamage = getEnhancedDamage();
            obj->takeDamage(enhancedDamage);
            std::cout << "💥 Direct damage spell hit " << obj->getName()
                      << " for " << enhancedDamage << " damage!\n";
            
            if (!obj->isAlive()) {
                std::cout << obj->getName() << " destroyed!\n";
                auto player = field.getPlayer();
                if (player) {
                    if (type == "Enemy") player->addScore(10);
                    else if (type == "EnemyBuilding") player->addScore(25);
                    else if (type == "EnemyTower") player->addScore(30);
                }
            }
            return true;
        } else {
            std::cout << "Cannot target friendly or neutral objects!\n";
            return false;
        }
    } else {
        std::cout << "No valid target at specified position!\n";
        return false;
    }
}

std::string DirectDamageSpell::getDescription() const {
    int enhancedDamage = getEnhancedDamage();
    int enhancedRadius = getEnhancedRadius();
    return "Deals " + std::to_string(enhancedDamage) + " damage to single target (radius: " + std::to_string(enhancedRadius) + ")";
}

void DirectDamageSpell::enhance() {
    damage += 5;
    enhancementLevel++;
}

int DirectDamageSpell::getEnhancedRadius() const {
    return radius + enhancementLevel;
}

int DirectDamageSpell::getEnhancedAreaSize() const {
    return 0;
}

int DirectDamageSpell::getEnhancedDamage() const {
    return damage + (enhancementLevel * 3);
}

int DirectDamageSpell::getEnhancedSummonCount() const {
    return 0;
}

// ========== AREA DAMAGE SPELL ==========

AreaDamageSpell::AreaDamageSpell(int damage, int radius, int areaSize)
    : name("Area Damage"), manaCost(25), enhancementLevel(0), damage(damage), radius(radius), areaSize(areaSize) {}

std::string AreaDamageSpell::getName() const { return name; }
int AreaDamageSpell::getManaCost() const { return manaCost; }

int AreaDamageSpell::getEnhancementLevel() const { return enhancementLevel; }
void AreaDamageSpell::setEnhancementLevel(int level) { enhancementLevel = level; }

bool AreaDamageSpell::cast(const Position& target, GameField& field, const Position& casterPos) {
    if (!field.isValidPosition(target)) {
        std::cout << "Invalid target position!\n";
        return false;
    }
    
    int distance = std::abs(target.x - casterPos.x) + std::abs(target.y - casterPos.y);
    int enhancedRadius = getEnhancedRadius();
    
    if (distance > enhancedRadius) {
        std::cout << "Target is too far! Maximum range: " << enhancedRadius << "\n";
        return false;
    }
    
    int enhancedAreaSize = getEnhancedAreaSize();
    int enhancedDamage = getEnhancedDamage();
    bool hitAnything = false;
    int hits = 0;
    
    int startX = target.x - enhancedAreaSize / 2;
    int startY = target.y - enhancedAreaSize / 2;
    int endX = target.x + enhancedAreaSize / 2;
    int endY = target.y + enhancedAreaSize / 2;
    
    std::cout << "🌀 Casting area damage spell (" << enhancedAreaSize << "x" << enhancedAreaSize << ") at ("
              << target.x << "," << target.y << ")\n";
    
    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            Position areaPos(x, y);
            if (field.isValidPosition(areaPos)) {
                auto& cell = field.getCell(areaPos);
                if (cell.hasObject()) {
                    auto obj = cell.getObject();
                    std::string type = obj->getType();
                    
                    if (type == "Enemy" || type == "EnemyBuilding" || type == "EnemyTower") {
                        obj->takeDamage(enhancedDamage);
                        hits++;
                        std::cout << "  Hit " << obj->getName() << " for " << enhancedDamage << " damage!\n";
                        hitAnything = true;
                        
                        if (!obj->isAlive()) {
                            std::cout << "  " << obj->getName() << " destroyed!\n";
                            auto player = field.getPlayer();
                            if (player) {
                                if (type == "Enemy") player->addScore(10);
                                else if (type == "EnemyBuilding") player->addScore(25);
                                else if (type == "EnemyTower") player->addScore(30);
                            }
                        }
                    }
                }
            }
        }
    }
    
    if (hits > 0) {
        std::cout << "Area spell hit " << hits << " targets!\n";
    } else {
        std::cout << "Area spell cast but no enemies in the area.\n";
    }
    
    return true;
}

std::string AreaDamageSpell::getDescription() const {
    int enhancedDamage = getEnhancedDamage();
    int enhancedRadius = getEnhancedRadius();
    int enhancedAreaSize = getEnhancedAreaSize();
    return "Deals " + std::to_string(enhancedDamage) + " damage in " +
           std::to_string(enhancedAreaSize) + "x" + std::to_string(enhancedAreaSize) +
           " area (radius: " + std::to_string(enhancedRadius) + ")";
}

void AreaDamageSpell::enhance() {
    areaSize += 1;
    damage += 3;
    enhancementLevel++;
}

int AreaDamageSpell::getEnhancedRadius() const {
    return radius + enhancementLevel;
}

int AreaDamageSpell::getEnhancedAreaSize() const {
    return areaSize + enhancementLevel;
}

int AreaDamageSpell::getEnhancedDamage() const {
    return damage + (enhancementLevel * 2);
}

int AreaDamageSpell::getEnhancedSummonCount() const {
    return 0;
}

// ========== TRAP SPELL ==========

TrapSpell::TrapSpell(int damage, int radius)
    : name("Trap"), manaCost(20), enhancementLevel(0), damage(damage), radius(radius) {}

std::string TrapSpell::getName() const { return name; }
int TrapSpell::getManaCost() const { return manaCost; }

int TrapSpell::getEnhancementLevel() const { return enhancementLevel; }
void TrapSpell::setEnhancementLevel(int level) { enhancementLevel = level; }

bool TrapSpell::cast(const Position& target, GameField& field, const Position& casterPos) {
    if (!field.isValidPosition(target)) {
        std::cout << "Invalid target position!\n";
        return false;
    }
    
    int distance = std::abs(target.x - casterPos.x) + std::abs(target.y - casterPos.y);
    int enhancedRadius = getEnhancedRadius();
    
    if (distance > enhancedRadius) {
        std::cout << "Target is too far! Maximum range: " << enhancedRadius << "\n";
        return false;
    }
    
    auto& cell = field.getCell(target);
    
    if (cell.getType() == CellType::IMPASSABLE) {
        std::cout << "Cannot place trap on impassable terrain!\n";
        return false;
    }
    
    if (cell.hasObject()) {
        std::cout << "Cannot place trap on occupied cell!\n";
        return false;
    }
    
    int enhancedDamage = getEnhancedDamage();
    auto trap = std::make_shared<Trap>(enhancedDamage);
    cell.setObject(trap);
    
    std::cout << "⚠️ Trap placed at (" << target.x << "," << target.y << ") - Damage: " << enhancedDamage << "\n";
    return true;
}

std::string TrapSpell::getDescription() const {
    int enhancedDamage = getEnhancedDamage();
    int enhancedRadius = getEnhancedRadius();
    return "Places trap that deals " + std::to_string(enhancedDamage) +
           " damage when triggered (radius: " + std::to_string(enhancedRadius) + ")";
}

void TrapSpell::enhance() {
    damage += 10;
    enhancementLevel++;
}

int TrapSpell::getEnhancedRadius() const {
    return radius + enhancementLevel;
}

int TrapSpell::getEnhancedAreaSize() const {
    return 0;
}

int TrapSpell::getEnhancedDamage() const {
    return damage + (enhancementLevel * 5);
}

int TrapSpell::getEnhancedSummonCount() const {
    return 0;
}

// ========== SUMMON SPELL ==========

SummonSpell::SummonSpell(int summonCount, int radius)
    : name("Summon Ally"), manaCost(30), enhancementLevel(0), summonCount(summonCount), radius(radius) {}

std::string SummonSpell::getName() const { return name; }
int SummonSpell::getManaCost() const { return manaCost; }

int SummonSpell::getEnhancementLevel() const { return enhancementLevel; }
void SummonSpell::setEnhancementLevel(int level) { enhancementLevel = level; }

bool SummonSpell::cast(const Position& target, GameField& field, const Position& casterPos) {
    if (!field.isValidPosition(target)) {
        std::cout << "Invalid target position!\n";
        return false;
    }
    
    int distance = std::abs(target.x - casterPos.x) + std::abs(target.y - casterPos.y);
    int enhancedRadius = getEnhancedRadius();
    
    if (distance > enhancedRadius) {
        std::cout << "Target is too far! Maximum range: " << enhancedRadius << "\n";
        return false;
    }
    
    int enhancedSummonCount = getEnhancedSummonCount();
    int summoned = 0;
    
    std::cout << " Attempting to summon " << enhancedSummonCount << " allies near ("
              << target.x << "," << target.y << ")\n";
    
    std::vector<Position> adjacentPositions = {
        {target.x-1, target.y}, {target.x+1, target.y},
        {target.x, target.y-1}, {target.x, target.y+1},
        {target.x-1, target.y-1}, {target.x+1, target.y-1},
        {target.x-1, target.y+1}, {target.x+1, target.y+1}
    };
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(adjacentPositions.begin(), adjacentPositions.end(), gen);
    
    for (const auto& pos : adjacentPositions) {
        if (summoned >= enhancedSummonCount) break;
        
        if (field.isValidPosition(pos)) {
            auto& cell = field.getCell(pos);
            if (cell.isPassable() && !cell.hasObject()) {
                int allyHealth = 40 + (enhancementLevel * 10);
                int allyDamage = 8 + (enhancementLevel * 2);
                auto ally = std::make_shared<Ally>(allyHealth, allyDamage);
                
                if (field.placeAlly(pos, ally)) {
                    summoned++;
                    std::cout << "   Summoned ally at (" << pos.x << "," << pos.y << ")\n";
                }
            }
        }
    }
    
    if (summoned > 0) {
        std::cout << "Successfully summoned " << summoned << " allies!\n";
        return true;
    } else {
        std::cout << " No suitable positions found for summoning!\n";
        return false;
    }
}

std::string SummonSpell::getDescription() const {
    int enhancedCount = getEnhancedSummonCount();
    int enhancedRadius = getEnhancedRadius();
    return "Summons " + std::to_string(enhancedCount) + " allies (radius: " + std::to_string(enhancedRadius) + ")";
}

void SummonSpell::enhance() {
    summonCount += 1;
    enhancementLevel++;
}

int SummonSpell::getEnhancedRadius() const {
    return radius + enhancementLevel;
}

int SummonSpell::getEnhancedAreaSize() const {
    return 0;
}

int SummonSpell::getEnhancedDamage() const {
    return 0;
}

int SummonSpell::getEnhancedSummonCount() const {
    return summonCount + enhancementLevel;
}

// ========== ENHANCEMENT SPELL ==========

EnhancementSpell::EnhancementSpell()
    : name("Enhancement"), manaCost(10), enhancementLevel(0), enhancementPower(1) {}

std::string EnhancementSpell::getName() const { return name; }
int EnhancementSpell::getManaCost() const { return manaCost; }

int EnhancementSpell::getEnhancementLevel() const { return enhancementLevel; }
void EnhancementSpell::setEnhancementLevel(int level) { enhancementLevel = level; }

bool EnhancementSpell::cast(const Position& target, GameField& field, const Position& casterPos) {
    std::cout << " Enhancement spell prepared! Next spell will be enhanced with power "
              << (enhancementPower + enhancementLevel) << "!\n";
    return true;
}

std::string EnhancementSpell::getDescription() const {
    int totalPower = enhancementPower + enhancementLevel;
    return "Enhances the next spell cast (Power: " + std::to_string(totalPower) + ")";
}

void EnhancementSpell::enhance() {
    enhancementPower += 1;
    enhancementLevel++;
}

bool EnhancementSpell::isEnhancement() const {
    return true;
}

int EnhancementSpell::getEnhancedRadius() const {
    return 0;
}

int EnhancementSpell::getEnhancedAreaSize() const {
    return 0;
}

int EnhancementSpell::getEnhancedDamage() const {
    return 0;
}

int EnhancementSpell::getEnhancedSummonCount() const {
    return 0;
}
