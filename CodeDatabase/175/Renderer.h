#pragma once
#include "GameState.h"
#include "Cell.h"
#include "Player.h"
#include "Hand.h"
#include <iostream>
#include <string>
#include <memory>
#ifdef _WIN32
#include <windows.h>
#endif

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render(const GameState& state) = 0;
    virtual void clear() = 0;
};

class ConsoleRenderer : public Renderer {
private:
    char getCellSymbol(const Cell& cell) const {
        if (cell.getPlayer()) return 'P';
        if (cell.getEnemy()) return 'E';
        if (cell.getBase()) return 'B';
        if (cell.getTower()) return 'T';
        if (cell.getAlly()) return 'A';
        if (cell.hasTrap()) return 'X';
        
        switch (cell.getType()) {
            case CellType::WALL: return '#';
            case CellType::SLOW: return '~';
            default: return '.';
        }
    }
    
public:
    void render(const GameState& state) override {
        if (auto field = state.getField()) {
            std::cout << "\n=== ИГРОВОЕ ПОЛЕ ===" << std::endl;
            for (int y = 0; y < field->getHeight(); ++y) {
                for (int x = 0; x < field->getWidth(); ++x) {
                    const auto& cell = field->getCell(x, y);
                    std::cout << getCellSymbol(cell) << " ";
                }
                std::cout << std::endl;
            }
        }
        
        if (auto player = state.getPlayer()) {
            std::cout << "\n=== ИНФОРМАЦИЯ ОБ ИГРОКЕ ===" << std::endl;
            std::cout << "HP: " << player->getHp() << "/" << player->getMaxHp() << std::endl;
            std::cout << "Урон: " << player->getDamage() << std::endl;
            std::cout << "Очки: " << player->getPoints() << std::endl;
            std::cout << "Режим атаки: " << 
                (player->getAttackMode() == AttackMode::MELEE ? "Ближний бой" : "Дальний бой") << std::endl;
            std::cout << "Дальность: " << player->getRangedRange() << std::endl;
            std::cout << "Замедлен: " << (player->isSlowed() ? "Да" : "Нет") << std::endl;
            
            auto spells = player->getHand().getAvailableSpells();
            if (!spells.empty()) {
                std::cout << "\n=== ЗАКЛИНАНИЯ ===" << std::endl;
                for (size_t i = 0; i < spells.size(); ++i) {
                    std::cout << i + 1 << ". " << spells[i]->getName() << std::endl;
                }
            }
        }
        
        std::cout << "\n=== ЛЕГЕНДА ===" << std::endl;
        std::cout << "P - Игрок, E - Враг, B - База, T - Башня, A - Союзник" << std::endl;
        std::cout << "# - Стена, ~ - Замедление, X - Ловушка, . - Пусто" << std::endl;
        std::cout << "\n=== УРОВЕНЬ " << state.getLevel() << " | ХОД " << state.getTurnCount() << " ===" << std::endl;
        std::cout << "\nКоманда: ";
    }
    
    void clear() override {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
};