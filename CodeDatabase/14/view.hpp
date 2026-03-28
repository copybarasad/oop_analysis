#ifndef VIEW_H
#define VIEW_H

#include "Game.hpp"
#include "defines.hpp"
#include <iostream>
#include <string>


class ConsoleRenderer {
public:
    template <typename InputT>
    void render(const Game& game, const InputT& input) {
        const GameBoard* board = game.getBoard(); 
        if (!board) return;

        system("cls"); 

        int h = board->getHeight();
        int w = board->getWidth();

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                Entity* e = board->getEntityAt(j, i);
                
                if (e != nullptr) {
                    switch (e->getEntityType()) {
                        case EntityType::PLAYER:   std::cout << "P "; break;
                        case EntityType::ALLY:     std::cout << "A "; break;
                        case EntityType::ENEMY:    std::cout << "E "; break;
                        case EntityType::TRAP:     std::cout << "@ "; break;
                        case EntityType::BUILDING: std::cout << "B "; break;
                        case EntityType::TOWER:    std::cout << "T "; break;
                    }
                } else {
                    if (board->isWall(j, i)) std::cout << "# ";
                    else if (board->isSlowing(j, i)) std::cout << "S ";
                    else std::cout << ". ";
                }
            }
            std::cout << std::endl;
        }
        
        std::cout << "----------------------------------------" << std::endl;
        
        Player* player = board->getPlayer();
        if (player) {
             std::cout << "Player HP: " << player->getHealth() 
                       << " | Score: " << player->getScore()
                       << " | Spells: " << player->getHand()->getSpellCount() << "/" << PLAYER_HAND_SIZE
                       << " | Enhancements: " << player->getEnhancementStacks()
                       << " | Attack Mode: " << (player->getAttackType() == Player::AttackType::MELEE ? "Melee" : "Ranged")
                       << " (Dmg: " << player->getDamage() << ")" << std::endl;
        }

        std::cout << "[" << input.getKey("MOVE_UP")   << "/" 
                         << input.getKey("MOVE_LEFT") << "/"
                         << input.getKey("MOVE_DOWN") << "/"
                         << input.getKey("MOVE_RIGHT")<< "] Move | "
                  << "[" << input.getKey("ATTACK")    << "] Attack | "
                  << "[" << input.getKey("SWITCH_WEAPON") << "] Weapon | "
                  << "[" << input.getKey("CAST_SPELL") << "] Cast | "
                  << "[" << input.getKey("SAVE") << "/" << input.getKey("LOAD") << "] Save/Load | "
                  << "[" << input.getKey("EXIT") << "] Quit" << std::endl;

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "--- Game Log ---" << std::endl;
        for(const auto& msg : board->getUILog()) {
            std::cout << "- " << msg << std::endl;
        }
        std::cout << "----------------------------------------" << std::endl;
    }
};

#endif