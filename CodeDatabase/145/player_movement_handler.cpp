#include "player_movement_handler.h"
#include "game.h"
#include "player.h"
#include "game_field.h"
#include "event_publisher.h"
#include "combat_system.h"
#include <iostream>
#include <algorithm>

void PlayerMovementHandler::moveUp(Game& game) {
    handleMovement(game, 0, -1);
}

void PlayerMovementHandler::moveDown(Game& game) {
    handleMovement(game, 0, 1);
}

void PlayerMovementHandler::moveLeft(Game& game) {
    handleMovement(game, -1, 0);
}

void PlayerMovementHandler::moveRight(Game& game) {
    handleMovement(game, 1, 0);
}

void PlayerMovementHandler::handleMovement(Game& game, int dx, int dy) {
    auto oldPos = game.player.getPosition();
    std::pair<int, int> newPos = {oldPos.first + dx, oldPos.second + dy};

    if (!canMoveToPosition(game, newPos.first, newPos.second)) {
        std::cout << "Cannot move there!\n";
        return;
    }

    executeMovement(game, newPos);
}

bool PlayerMovementHandler::canMoveToPosition(const Game& game, int x, int y) {
    if (!game.field.isValidPosition(x, y) || !game.field.isPassable(x, y)) {
        return false;
    }

    for (const auto& enemy : game.enemies) {
        if (enemy.isAlive() && enemy.getPosition() == std::make_pair(x, y)) {
            return false;
        }
    }

    return true;
}

void PlayerMovementHandler::executeMovement(Game& game, const std::pair<int, int>& newPos) {
    auto oldPos = game.player.getPosition();
    game.player.setPosition(newPos.first, newPos.second);

    EventPublisher::publishMoveEvent("Player", oldPos, newPos);

    if (game.field.isSlowing(newPos.first, newPos.second)) {
        std::cout << "Player stepped on slowing cell! Will skip next turn.\n";
        game.playerSkippingTurn = true;
    }

    // Check if there are adjacent enemies and attack them if in MELEE mode
    if (game.player.getCombatMode() == CombatMode::MELEE) {
        for (auto& enemy : game.enemies) {
            if (!enemy.isAlive()) continue;
            
            auto enemyPos = enemy.getPosition();
            int dx = std::abs(newPos.first - enemyPos.first);
            int dy = std::abs(newPos.second - enemyPos.second);
            
            // If enemy is adjacent (manhattan distance = 1)
            if ((dx + dy) == 1) {
                std::cout << "Melee attack! Player deals " << game.player.getMeleeDamage() 
                          << " damage to enemy at (" << enemyPos.first << "," << enemyPos.second << ")!\n";
                enemy.takeDamage(game.player.getMeleeDamage());
                
                if (enemy.isAlive()) {
                    std::cout << "Enemy HP: " << enemy.getHealth() << "/" << enemy.getMaxHealth() << "\n";
                } else {
                    std::cout << "Enemy defeated! +10 points\n";
                    game.player.increaseScore(10);
                }
            }
        }
    }

    if (!game.player.isAlive()) {
        game.gameOver = true;
    }
}
