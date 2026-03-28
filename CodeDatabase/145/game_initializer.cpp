#include "game_initializer.h"
#include "game.h"
#include "exceptions.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include "summon_spell.h"
#include "enhancement_spell.h"
#include <iostream>
#include <cstdlib>

void GameInitializer::initializeNewGame(Game& game) {
    try {
        game.player = Player(7, 7);
        game.field.setCell(7, 7, CellType::EMPTY);
        
        spawnInitialEnemies(game, 3);
        spawnInitialBuildings(game, 2);
        spawnInitialTowers(game, 1);
        initializeSpells(game);
    } catch (const std::exception& ex) {
        throw GameLogicException(std::string("Failed to initialize new game: ") + ex.what());
    }
}

void GameInitializer::spawnInitialEnemies(Game& game, int count) {
    if (count <= 0) {
        throw GameLogicException("Cannot spawn " + std::to_string(count) + " enemies: count must be positive");
    }
    try {
        for (int i = 0; i < count; ++i) {
            auto pos = game.generateRandomValidPosition();
            int baseHealth = 20;
            int baseDamage = 8;
            int health = baseHealth + (game.currentLevel - 1) * 5;
            int damage = baseDamage + (game.currentLevel - 1) * 2;
            game.enemies.push_back(Enemy(health, damage, pos.first, pos.second));
        }
    } catch (const std::exception& ex) {
        throw GameLogicException(std::string("Failed to spawn ") + std::to_string(count) + " initial enemies: " + ex.what());
    }
}

void GameInitializer::spawnInitialBuildings(Game& game, int count) {
    if (count <= 0) {
        throw GameLogicException("Cannot spawn " + std::to_string(count) + " buildings: count must be positive");
    }
    try {
        for (int i = 0; i < count; ++i) {
            auto pos = game.generateRandomValidPosition();
            game.buildings.push_back(EnemyBuilding(pos.first, pos.second, 15));
        }
    } catch (const std::exception& ex) {
        throw GameLogicException(std::string("Failed to spawn ") + std::to_string(count) + " initial buildings: " + ex.what());
    }
}

void GameInitializer::spawnInitialTowers(Game& game, int count) {
    if (count <= 0) {
        throw GameLogicException("Cannot spawn " + std::to_string(count) + " towers: count must be positive");
    }
    try {
        for (int i = 0; i < count; ++i) {
            auto pos = game.generateRandomValidPosition();
            game.towers.push_back(EnemyTower(pos.first, pos.second, 5, 5, 2));
        }
    } catch (const std::exception& ex) {
        throw GameLogicException(std::string("Failed to spawn ") + std::to_string(count) + " initial towers: " + ex.what());
    }
}

void GameInitializer::initializeSpells(Game& game) {
    try {
        std::vector<std::unique_ptr<Spell>> availableSpells;
        availableSpells.push_back(std::make_unique<DirectDamageSpell>(10, 3));
        availableSpells.push_back(std::make_unique<AreaDamageSpell>(8, 4, 2));
        availableSpells.push_back(std::make_unique<TrapSpell>(15, 4));
        availableSpells.push_back(std::make_unique<SummonSpell>(1, 15, 5));
        availableSpells.push_back(std::make_unique<EnhancementSpell>());
        
        if (availableSpells.empty()) {
            throw ResourceException("No available spells to initialize");
        }
        
        int randomIndex = rand() % availableSpells.size();
        game.spellManager->addSpellToHand(std::move(availableSpells[randomIndex]));
    } catch (const ResourceException&) {
        throw;
    } catch (const std::exception& ex) {
        throw GameLogicException(std::string("Failed to initialize spells: ") + ex.what());
    }
}

void GameInitializer::displayControls() {
    std::cout << "Controls:\n";
    std::cout << "W/A/S/D - Move up/left/down/right\n";
    std::cout << "M - Switch combat mode (Melee/Ranged)\n";
    std::cout << "X - Ranged attack\n";
    std::cout << "0-2 - Use spell from hand (enter target coordinates after)\n";
    std::cout << "Z - Save game\n";
    std::cout << "L - Load game\n";
    std::cout << "Q - Quit game\n\n";
}