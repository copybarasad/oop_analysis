#include "collision_handler.h"
#include "enemy.h"
#include "ally.h"
#include "player.h"
#include "game.h"
#include <iostream>

void CollisionHandler::handleEnemyPlayerCollision(Enemy& enemy, Player& player, Game& game) {
    int damage = enemy.getDamage();
    player.takeDamage(damage);

    std::cout << "Enemy collided with player! Player takes " << damage << " damage!\n";
    std::cout << "Player HP: " << player.getHealth() << "/" << player.getMaxHealth() << "\n";
}

void CollisionHandler::handleAllyEnemyCollision(Ally& ally, Enemy& enemy, Player& player) {
    int damage = ally.getDamage();
    enemy.takeDamage(damage);

    std::cout << "Ally attacked enemy for " << damage << " damage!\n";

    if (!enemy.isAlive()) {
        std::cout << "Enemy defeated by ally! +5 points\n";
        player.increaseScore(5);
    }
}

void CollisionHandler::handleTrapCollision(const std::pair<int, int>& position, Game& game) {
    if (game.field.hasTrapAt(position.first, position.second)) {
        int damage = game.field.getTrapDamageAt(position.first, position.second);
        game.player.takeDamage(damage);

        std::cout << "Player stepped on trap and takes " << damage << " damage!\n";
        std::cout << "Player HP: " << game.player.getHealth() << "/"
                  << game.player.getMaxHealth() << "\n";

        game.field.removeTrap(position.first, position.second);
    }
}

void CollisionHandler::handleEnemyPlayerCollision(Enemy& enemy, Player& player) {
    int damage = enemy.getDamage();
    player.takeDamage(damage);

    std::cout << "Enemy collided with player! Player takes " << damage << " damage!\n";
    std::cout << "Player HP: " << player.getHealth() << "/" << player.getMaxHealth() << "\n";
}