#include "game_turn_processor.h"
#include "game.h"
#include "player_movement_handler.h"
#include "spell_context_factory.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include "summon_spell.h"
#include "enhancement_spell.h"
#include "event_bus.h"
#include "game_event.h"
#include "event_publisher.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>

void GameTurnProcessor::processPlayerTurn(Game& game, char command) {
    if (game.playerSkippingTurn) {
        std::cout << "Player is slowed and skips this turn!\n";
        game.playerSkippingTurn = false;
        game.playerTurnCounter++;
        return;
    }
    game.playerTurnCounter++;

    if (command >= '0' && command <= '3') {
        int spellIndex = command - '0';
        const auto& spells = game.spellManager->getHand().getSpells();

        if (spellIndex >= (int)spells.size()) {
            std::cout << "Invalid spell index!\n";
            return;
        }

        int targetX = game.player.getPosition().first;
        int targetY = game.player.getPosition().second;

        bool needsPrompt = true;
        if (spells[spellIndex]) {
            const char* name = spells[spellIndex]->getName();
            if (name && std::string(name) == "Summon") needsPrompt = false;
        }
        if (needsPrompt) {
            std::cout << "Enter target X: ";
            std::cin >> targetX;
            std::cout << "Enter target Y: ";
            std::cin >> targetY;
        }

        if (spellIndex < (int)spells.size() && spells[spellIndex]) {
            auto* spell = spells[spellIndex].get();
            bool isEnhancement = (std::string(spell->getName()) == "Enhancement");

            if (!isEnhancement && game.enhancementLevel > 0) {
                std::cout << "Applying " << game.enhancementLevel << " enhancement(s)...\n";
                SpellContextFactory::applyEnhancements(*spell, game.enhancementLevel);
                game.enhancementLevel = 0;
            }

            if (SpellContextFactory::applySpellWithContext(*spell, game.player, game.field,
                                                           game.enemies, game.buildings, game.towers,
                                                           game.allies, targetX, targetY)) {
                if (isEnhancement) {
                    game.enhancementLevel++;
                    std::cout << "Enhancement accumulated (level " << game.enhancementLevel << ")\n";
                }
                game.cleanupDestroyedBuildings();
                game.cleanupDestroyedTowers();
            } else {
                std::cout << "Cannot use spell at target!\n";
            }
        }
        return;
    }

    switch (command) {
        case 'w': case 'W':
            PlayerMovementHandler::moveUp(game);
            return;
        case 's': case 'S':
            PlayerMovementHandler::moveDown(game);
            return;
        case 'a': case 'A':
            PlayerMovementHandler::moveLeft(game);
            return;
        case 'd': case 'D':
            PlayerMovementHandler::moveRight(game);
            return;
        case 'm': case 'M':
            game.player.switchCombatMode();
            std::cout << "Switched to " <<
                (game.player.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED") << " mode\n";
            std::cout << "Mode switch consumed turn!\n";
            return;
        case 'x': case 'X':
            if (game.player.getCombatMode() == CombatMode::RANGED) {
                for (auto& tower : game.towers) {
                    int dx = std::abs(game.player.getPosition().first - tower.getPosition().first);
                    int dy = std::abs(game.player.getPosition().second - tower.getPosition().second);
                    if ((dx + dy) == 1 && tower.isAlive()) {
                        tower.takeDamage(game.player.getRangedDamage());
                        std::cout << "Player attacks tower in ranged for " << game.player.getRangedDamage() << " damage!\n";
                    }
                }
                game.cleanupDestroyedTowers();
                game.combatSystem->performRangedAttack();
                game.cleanupDestroyedBuildings();
                return;
            } else {
                std::cout << "Melee combat is automatic when enemies are adjacent!\n";
            }
            break;
        default:
            std::cout << "Invalid command!\n";
            return;
    }
}

void GameTurnProcessor::processAlliesTurn(Game& game) {
    for (auto& ally : game.allies) {
        if (!ally.isAlive()) continue;

        auto oldPos = ally.getPosition();
        auto newPos = ally.calculateMoveTowardsClosestEnemy(game.enemies, game.field, game.buildings, game.towers);

        bool blocked = false;
        for (auto& enemy : game.enemies) {
            if (!enemy.isAlive()) continue;
            if (enemy.getPosition() == newPos) {
                int damage = ally.getDamage();
                enemy.takeDamage(damage);
                std::cout << "Ally attacked enemy for " << damage << " damage!\n";
                if (!enemy.isAlive()) {
                    std::cout << "Enemy defeated by ally! +5 points\n";
                    game.player.increaseScore(5);
                }
                blocked = true;
                break;
            }
        }

        if (!blocked && newPos != oldPos) {
            if (!game.field.isOccupied(newPos.first, newPos.second, game.enemies, game.buildings, game.towers, &game.player, &game.allies)) {
                ally.setPosition(newPos.first, newPos.second);

                if (game.field.hasTrapAt(newPos.first, newPos.second)) {
                    int trapDamage = game.field.getTrapDamageAt(newPos.first, newPos.second);
                    ally.takeDamage(trapDamage);
                    std::cout << "Ally stepped on trap and took " << trapDamage << " damage!\n";
                    game.field.removeTrap(newPos.first, newPos.second);
                }

                for (auto& enemy : game.enemies) {
                    if (!enemy.isAlive()) continue;
                    auto enemyPos = enemy.getPosition();
                    int dx = std::abs(newPos.first - enemyPos.first);
                    int dy = std::abs(newPos.second - enemyPos.second);

                    if ((dx + dy) == 1) {
                        int damage = ally.getDamage();
                        enemy.takeDamage(damage);
                        std::cout << "Ally attacked enemy for " << damage << " damage!\n";
                        if (!enemy.isAlive()) {
                            std::cout << "Enemy defeated by ally! +5 points\n";
                            game.player.increaseScore(5);
                        }
                    }
                }
            }
        }
        else if (blocked) {
            for (auto& enemy : game.enemies) {
                if (!enemy.isAlive()) continue;
                auto enemyPos = enemy.getPosition();
                int dx = std::abs(oldPos.first - enemyPos.first);
                int dy = std::abs(oldPos.second - enemyPos.second);

                if ((dx + dy) == 1) {
                    if (enemyPos != newPos) {
                        int damage = ally.getDamage();
                        enemy.takeDamage(damage);
                        std::cout << "Ally attacked enemy for " << damage << " damage!\n";
                        if (!enemy.isAlive()) {
                            std::cout << "Enemy defeated by ally! +5 points\n";
                            game.player.increaseScore(5);
                        }
                    }
                }
            }
        }
    }

    game.allies.erase(std::remove_if(game.allies.begin(), game.allies.end(),
        [](const Ally& a) { return !a.isAlive(); }), game.allies.end());
}

void GameTurnProcessor::processEnemyTurns(Game& game) {
    game.enemyManager->moveEnemies(game.buildings, game.towers);
    
    // Проверяем, жив ли игрок после атак врагов
    if (!game.player.isAlive()) {
        game.gameOver = true;
        return;
    }

    // Удаляем мёртвых врагов (как от урона при контакте, так и от ловушек)
    game.enemies.erase(std::remove_if(game.enemies.begin(), game.enemies.end(),
        [](const Enemy& e) { return !e.isAlive(); }), game.enemies.end());

    game.cleanupDestroyedBuildings();
}

void GameTurnProcessor::processBuildingTurns(Game& game) {
    for (auto& building : game.buildings) {
        building.update();
    }
    game.enemyManager->processBuildingSpawns(game.buildings, game.towers);
    game.cleanupDestroyedBuildings();
}

void GameTurnProcessor::processTowerTurns(Game& game) {
    auto playerPos = game.player.getPosition();
    
    for (auto& tower : game.towers) {
        tower.update();
        
        int dx = std::abs(playerPos.first - tower.getPosition().first);
        int dy = std::abs(playerPos.second - tower.getPosition().second);
        int manhattan = dx + dy;
        if (manhattan <= 3 && tower.canAttack()) {
            int damage = tower.getSpellDamage();
            game.player.takeDamage(damage);
            tower.performAttack();
            std::cout << "Enemy tower attacked player for " << damage << " damage!\n";
            if (!game.player.isAlive()) {
                game.gameOver = true;
            }
        }
    }
    game.cleanupDestroyedTowers();
    game.cleanupDestroyedBuildings();
}

bool GameTurnProcessor::processFullTurn(Game& game, char command) {
    try {
        processPlayerTurn(game, command);
        if (game.gameOver) return false;

        processAlliesTurn(game);
        if (game.gameOver) return false;

        processEnemyTurns(game);
        if (game.gameOver) return false;

        processBuildingTurns(game);
        if (game.gameOver) return false;

        processTowerTurns(game);
        if (game.gameOver) return false;

        return true;
    } catch (const std::exception& ex) {
        game.setLastError(std::string("Runtime error during turn processing: ") + ex.what());
        std::cout << "Error: " << game.getLastError() << "\n";
        return true;
    }
}