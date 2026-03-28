#include "console_renderer.h"
#include <iostream>
#include <iomanip>

void ConsoleRenderer::RenderField(const GameField& field, const Player& player, const EnemyManager& em) {
    std::cout << "================================ Поле =================================\n";
    
    int width = field.GetWidth();
    int height = field.GetHeight();
    
    std::cout << "   "; 
    
    for (int x = 0; x < width; ++x) {
        int col_num = x;
        if (col_num < 10) {
            std::cout << " " << col_num << " ";
        } else if (col_num < 100) {
            std::cout << col_num << " ";
        } else {
            std::cout << col_num;
        }
        col_num++;
    }
    std::cout << "\n";
    
    for (int y = 0; y < height; ++y) {   
        int row_num = y;     
        if (row_num < 10) {
            std::cout << " " << row_num << " ";
        } else if (row_num < 100) {
            std::cout << row_num << " ";
        } else {
            std::cout << row_num;
        }
        
        row_num++;

        for (int x = 0; x < width; ++x) {
            Position pos{x, y};
            char display_char = '.';
            
            if (player.GetPosition() == pos) {
                display_char = 'P';
            }
            else {
                for (const auto& e : em.GetEnemies()) {
                    if (e.IsAlive() && e.GetPosition() == pos) {
                        display_char = 'E';
                        break;
                    }
                }
            }
            if (display_char == '.') {
                for (const auto& tower : em.GetTowers()) {
                    if (tower.IsAlive() && tower.GetPosition() == pos) {
                        display_char = 'C';
                        break;
                    }
                }
            }
            if (display_char == '.' && field.GetCellType(pos) == CellType::Obstacle) {
                display_char = '#';
            }
            else if (display_char == '.' && field.HasTrap(pos)) {
                display_char = 'T';
            }
            
            std::cout << " " << display_char << " ";
        }
        std::cout << '\n';
    }
}

void ConsoleRenderer::RenderStatus(const Player& player, const EnemyManager& em) {
    std::cout << "=============================== Статус ================================\n";
    std::cout << "Имя: " << player.GetName() << "\n";
    std::cout << "HP: " << player.GetHealth() << "/" << player.GetMaxHealth() << "\n";
    std::cout << "Режим боя: "
              << (player.GetCombatMode() == CombatMode::Melee ? "Ближний" : "Дальний") << "\n";
    std::cout << "Очки: " << player.GetScore() << "\n";
    std::cout << "Позиция: (" << player.GetPosition().x << "," << player.GetPosition().y << ")\n";

    std::cout << "Рука заклинаний (" << player.GetHand().GetSize() 
              << "/" << player.GetHand().GetMaxSize() << "):\n";
    for (size_t i = 0; i < player.GetHand().GetSize(); ++i) {
        const Spell* spell = player.GetHand().GetSpell(i);
        if (spell) {
            std::cout << "  " << (i+1) << ": " << spell->GetName() << "\n";
        }
    }
    if (player.GetHand().GetSize() == 0) {
        std::cout << "  (пусто)\n";
    }
    
    int aliveCount = 0;
    for (const auto& e : em.GetEnemies()) {
        if (e.IsAlive())
            std::cout << "Враг " << ++aliveCount << ": " << e.GetHealth()
                      << " HP на позиции (" << e.GetPosition().x << "," << e.GetPosition().y << ")\n";
    }

    int towerCount = 0;
    for (const auto& tower : em.GetTowers()) {
        if (tower.IsAlive()) {
            std::cout << "Башня " << ++towerCount << ": " << tower.GetHealth()
                      << " HP на позиции (" << tower.GetPosition().x << "," << tower.GetPosition().y << ")\n";
        }
    }

    std::cout << "=======================================================================\n";
}

void ConsoleRenderer::RenderWelcome() {
    std::cout << "Добро пожаловать в игру!\n";
    std::cout << "Уничтожьте всех врагов на каждом уровне!\n\n";
}

void ConsoleRenderer::RenderLevelStart(int level, int enemy_count) {
    std::cout << "\n\nЗдоровье восстановлено: 100/100\n";
    std::cout << "\nЗапуск уровня " << level << std::endl;
    std::cout << "Врагов на уровне: " << enemy_count << std::endl;
}

void ConsoleRenderer::RenderGameResult(const GameField& field, const Player& player, const EnemyManager& em) {
    std::cout << "\n======================== Финальное состояние =========================\n\n";
    RenderField(field, player, em);
    RenderStatus(player, em);
    std::cout << (player.IsAlive() ? "Вы победили всех врагов!\n" : "Вы погибли...\n");
}
