#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "field.h"
#include "player.h"
#include <iostream>
#include <vector>
#include <string>

class GameRenderer {
public:
    virtual ~GameRenderer() = default;
    
    virtual void renderField(Field* field) = 0;
    virtual void renderGameState(int hp, int max_hp, int mana, int points, 
                                 int enemies, int buildings, int turn, int level,
                                 const std::vector<std::string>& spells) = 0;
    virtual void renderMessage(const std::string& message) = 0;
    virtual void clearScreen() = 0;
};

class ConsoleRenderer : public GameRenderer {
public:
    void renderField(Field* field) {
        if (field) {
            field->print_field();
        }
    }
    
    void renderGameState(int hp, int max_hp, int mana, int points, 
                        int enemies, int buildings, int turn, int level,
                        const std::vector<std::string>& spells) {
        std::cout << "\n=== ИГРОВАЯ ИНФОРМАЦИЯ ===\n";
        std::cout << "Уровень: " << level << " | Ход: " << turn << "\n";
        std::cout << "HP: " << hp << "/" << max_hp << " | Мана: " << mana << "\n";
        std::cout << "Очки до заклинания: " << points << "/5\n";
        std::cout << "Врагов: " << enemies << " | Зданий: " << buildings << "\n";
        std::cout << "Заклинания: ";
        for (const auto& spell : spells) {
            std::cout << spell << " ";
        }
        std::cout << "\n========================\n\n";
    }
    
    void renderMessage(const std::string& message) {
        std::cout << message << "\n";
    }
    
    void clearScreen() {
        std::cout << "\033[2J\033[1;1H";
    }
};

#endif