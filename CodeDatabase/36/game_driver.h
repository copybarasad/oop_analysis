#ifndef GAME_DRIVER_H
#define GAME_DRIVER_H

#include "world.h"
#include <iostream>

// TInputHandler - класс для считывания ввода
// TVisualizer - класс для визуализации
template <typename TInputHandler, typename TVisualizer>
class GameDriver {
public:
    // Принимает мир по ссылке (Dependency Injection), не создает его сам
    explicit GameDriver(World& world) : world_(world) {}

    void Run() {
        bool running = true;
        
        // Основной игровой цикл
        while (running && !world_.IsGameOver()) {
            
            // В начале хода у игрока 2 очка действий
            int actions_left = 2;

            // Отрисовываем мир перед началом хода
            visualizer_.Update(world_);

            // Цикл действий игрока (пока есть очки действий и игра идет)
            while (actions_left > 0 && running && !world_.IsGameOver()) {
                
                // 1. Получение команды
                auto command = input_handler_.GetCommand(world_);

                if (command->IsQuit()) {
                    running = false;
                    break;
                }

                // 2. Выполнение команды
                // Execute возвращает true, только если действие было игровым (Move, Attack, Cast)
                // Save, Load, SwitchMode возвращают false
                bool action_consumed = command->Execute(world_);
                
                if (action_consumed) {
                    actions_left--;
                    
                    // Если у нас еще осталось действие и игра не закончена,
                    // нужно перерисовать мир (чтобы игрок видел результат первого действия перед вторым)
                    if (actions_left > 0 && !world_.IsGameOver()) {
                        visualizer_.Update(world_);
                        std::cout << "Actions left: " << actions_left << "\n";
                    }
                }
                
                // Если команда была Load (загрузка), состояние мира изменилось.
                // В текущей реализации мы продолжаем ход с оставшимися очками действий.
            }

            // 3. После того как игрок потратил 2 действия (или вышел), ходят враги
            if (running && !world_.IsGameOver()) {
                std::cout << "Enemies turn...\n";
                world_.AdvanceTurn();
                CheckLevelUp();
            }
        }

        std::cout << "Game Over!\n";
    }

private:
    void CheckLevelUp() {
        if (!world_.IsBaseAlive()) {
            std::cout << "Level Completed!\n";
            
            Player upgraded_player = std::move(world_.MutablePlayer());
            upgraded_player.LevelUp();
            
            int choice = input_handler_.GetUpgradeChoice();
            switch (choice) {
                case 1: upgraded_player.UpgradeHealth(); break;
                case 2: upgraded_player.UpgradeDamage(); break;
                case 3: upgraded_player.UpgradeMaxSpells(); break;
                case 4: upgraded_player.UpgradeEnhancementStack(); upgraded_player.ConsumeEnhancementStack(); break;
            }

            int next_level = world_.GetLevel() + 1;
            int new_w = std::min(50, 12 + 2 * (next_level - 1));
            int new_h = std::min(50, 12 + 2 * (next_level - 1));
            
            world_ = World(new_w, new_h, next_level, std::move(upgraded_player));
        }
    }

    World& world_;
    TInputHandler input_handler_;
    TVisualizer visualizer_;
};

#endif