#include "ShowStatusCommand.h"
#include "GameTypes.h" 
#include "Game.h"
#include <iostream>

bool ShowStatusCommand::execute(Game* game) {
    if (!game) return false;
    
    Player* player = game->getPlayer();
    if (!player) return false;
    
    // Выводим статус игрока
    std::cout << "\n=== PLAYER STATUS ===\n";
    std::cout << "Health: " << player->getHealth() << "\n";
    std::cout << "Damage: " << player->getDamage() << "\n";
    std::cout << "Position: (" << player->getPositionX() << ", " << player->getPositionY() << ")\n";
    std::cout << "Score: " << player->getScore() << "\n";
    std::cout << "Enemies Killed: " << player->getEnemiesKilled() << "\n";
    std::cout << "Mode: " << (player->getAttackMode() == AttackMode::MELEE ? "MELEE" : "RANGED") << "\n";
    
    // Выводим информацию об игре
    std::cout << "\n=== GAME INFO ===\n";
    std::cout << "Enemies: " << game->getEnemies().size() << "\n";
    std::cout << "Towers: " << game->getTowers().size() << "\n";
    std::cout << "Allies: " << game->getAllies().size() << "\n";
    std::cout << "=====================\n\n";
    
    return true;
}
std::string ShowStatusCommand::getDescription() const {
    return "Show player and game status";
}


