#ifndef RENDERER_H
#define RENDERER_H

#include "Game.h"
#include "GameController.h"
#include "GameField.h"
#include "Player.h"
#include "Cell.h"
#include <iostream>
#include <cstdlib>
#include <string>

// Консольная реализация рендерера 
class ConsoleRenderer {
private:
    void clear() {
        system("clear");
    }
    
    std::string getCellSymbol(const Game& game, int x, int y) {
        const GameController* ctrl = game.getController();
        if (!ctrl) return ". ";
        
        const GameField& field = ctrl->getField();
        Position pos(x, y);
        
        if (pos == field.getPlayerPosition()) return "P ";
        if (field.hasCharacterAt(pos)) {
            const Character* ch = field.viewCharacterAt(pos);
            return (ch && ch->isAlive()) ? "E " : ". ";
        }
        if (field.hasTowerAt(pos)) return "T ";
        if (field.hasTrapAt(pos)) return "* ";
        
        return field.getCellType(pos) == CellType::WALL ? "# " : ". ";
    }
    
public:
    ConsoleRenderer() = default;
    
    // Основной метод отрисовки
    void render(const Game& game) {
        clear();
        
        switch (game.getStatus()) {
            case GameStatus::MENU:
                renderMenu();
                break;
            case GameStatus::PLAYING:
                renderGame(game);
                break;
            case GameStatus::GAME_OVER:
                renderGameOver(game.getScore());
                break;
            case GameStatus::LEVEL_COMPLETE:
                renderLevelComplete(game.getLevel(), game.getScore());
                break;
            default:
                std::cout << "Неизвестное состояние игры\n";
        }
    }
    
    void renderGame(const Game& game) {
        const GameController* ctrl = game.getController();
        if (!ctrl) return;
        
        const GameField& field = ctrl->getField();
        const Player& player = ctrl->getPlayer();
        
        std::cout << "Уровень:" << game.getLevel() 
                  << " Здоровье:" << player.getHealth() << "/" << player.getMaxHealth()
                  << " Очки:" << player.getScore() 
                  << " " << (player.getCombatMode() == CombatMode::MELEE ? "БЛИЖНИЙ" : "ДАЛЬНИЙ")
                  << " Урон:" << player.getCurrentDamage() << "\n";
        
        std::cout << "Заклинания: ";
        for (int i = 0; i < player.getHand().getSpellCount(); ++i) {
            const Spell* spell = player.getHand().getSpell(i);
            if (spell) {
                std::cout << (i + 1) << ":" << spell->getName() << " ";
            }
        }
        std::cout << "\n";
        
        if (ctrl->isAwaitingSpellTarget()) {
            std::cout << "Выберите цель: WASD (Q для отмены)\n";
        }
        
        for (int y = 0; y < field.getHeight(); ++y) {
            for (int x = 0; x < field.getWidth(); ++x) {
                std::cout << getCellSymbol(game, x, y);
            }
            std::cout << "\n";
        }
        
        std::cout << "WASD:Движение  M:Режим  1/2:Заклинания  5:Сохранить  6:Загрузить  Q:Меню\n";
        
        if (player.getHealth() <= 30) {
            std::cout << "МАЛО ЗДОРОВЬЯ!\n";
        }
    }
    
    void renderMenu() {
        std::cout << "=== ПОДЗЕМЕЛЬНЫЙ ИССЛЕДОВАТЕЛЬ ===\n"
                  << "1. Новая игра\n"
                  << "2. Загрузить игру\n"
                  << "3. Выход\n"
                  << "===================================\n";
    }
    
    void renderGameOver(int score) {
        std::cout << "=== ИГРА ОКОНЧЕНА ===\n"
                  << "Итоговый счет: " << score << "\n"
                  << "1. Попробовать снова\n"
                  << "2. Главное меню\n"
                  << "=====================\n";
    }
    
    void renderLevelComplete(int level, int score) {
        std::cout << "=== УРОВЕНЬ " << level << " ПРОЙДЕН! ===\n"
                  << "Счет: " << score << "\n"
                  << "1. Продолжить на следующий уровень\n"
                  << "2. Сохранить и вернуться в меню\n"
                  << "================================\n";
    }
};

#endif