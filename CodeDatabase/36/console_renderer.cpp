#include "console_renderer.h"
#include <iostream>

// Инициализация конфига (он сам загрузится из файла)
ConsoleRenderer::ConsoleRenderer() : key_config_() {}

void ConsoleRenderer::Render(const World& world) {
    std::cout << "\n";
    PrintMap(world.GetField());
    PrintStats(world);
    PrintControls(); // Вызов вывода управления
    std::cout << "=========================================\n";
}

void ConsoleRenderer::PrintMap(const Field& field) {
    // ... (без изменений) ...
    int w = field.GetWidth();
    int h = field.GetHeight();

    std::cout << "   ";
    for (int i = 0; i < w; i++) std::cout << " " << i % 10;
    std::cout << "\n";

    for (int r = 0; r < h; ++r) {
        std::cout << r % 10 << "  ";
        for (int c = 0; c < w; ++c) {
            char ch = '_';
            const Cell& cell = field.CellAt(r, c);
            switch (cell.GetType()) {
                case CellType::Player: ch = 'P'; break;
                case CellType::Enemy: ch = 'E'; break;
                case CellType::Wall: ch = '#'; break;
                case CellType::EnemyBase: ch = 'B'; break;
                case CellType::Slow: ch = '~'; break;
                case CellType::Trap: ch = 'T'; break;
                case CellType::Ally: ch = 'A'; break;
                case CellType::Tower: ch = 'W'; break;
                default: ch = '_';
            }
            std::cout << ch << " ";
        }
        std::cout << "\n";
    }
}

void ConsoleRenderer::PrintStats(const World& world) {
    const Player& p = world.GetPlayer();
    std::cout << "\nHP: " << p.Health() << "/" << p.MaxHealth() 
              << " | Dmg: " << p.CurrentDamage()
              << " | Spells: " << p.GetSpellCount()
              << " | Mode: " << (p.IsRanged() ? "Ranged" : "Melee")
              << " | Turn: " << world.GetTurn() << "\n";
}

// НОВЫЙ МЕТОД
void ConsoleRenderer::PrintControls() {
    std::cout << "-----------------------------------------\n";
    std::cout << key_config_.GetHelpString() << "\n";
}