#include "rendering.h"
#include "Game.h"
#include <iostream>

void ConsoleRenderer::render(Game& game) {
    const Player& player = game.getPlayer();
    const Hand& hand = game.getHand();
    const Field& field = game.getField(); 
    
    field.print_field(); 
    
    std::cout << "\n------------------------------------------------\n";
    std::cout << "УРОВЕНЬ: " << game.getLevel() << "\n"; 
    
    std::cout << "HP: " << player.getCurrentHP() 
              << " | KILLS: " << player.getKills() << "\n";
              
    std::cout << "ВРАГОВ: " << game.getCounterEnemies() 
              << " | БАШЕН: " << game.getCounterTowers() << "\n";
    
    std::cout << "СТИЛЬ: " << (player.getIsMelee() ? "Ближний" : "Дальний") 
              << " (Урон: " << player.getCurrentDamage() 
              << ", Дальность: " << player.getCurrentRange() << ")\n";
    
    hand.print_hand(); 
    std::cout << "------------------------------------------------\n";
}