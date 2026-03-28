#include "ConsoleRenderer.h"
#include <iostream>

void ConsoleRenderer::print_log(const std::string& msg) {
    std::cout << "[LOG]: " << msg << std::endl;
}

void ConsoleRenderer::render_game(Field& field, Player& player) {
    std::string s;
    s += "\n  ";
    for (int x = 0; x < field.getWidth(); ++x) s += std::to_string(x % 10) + " ";
    s += '\n';

    for (int y = 0; y < field.getHeight(); ++y) {
        s += std::to_string(y % 10) + " ";
        for (int x = 0; x < field.getWidth(); ++x) {
            switch (field.getCellType({x, y})) {
                case Field::Cell::Empty: s += "·"; break;
                case Field::Cell::Wall: s += "█"; break;
                case Field::Cell::Player: s += "@"; break;
                case Field::Cell::Enemy: s += "E"; break;
                case Field::Cell::Trap: s += "▲"; break;
                case Field::Cell::Tower: s += "T"; break;
                default: s += "?"; break;
            }
            s += " ";
        }
        s += std::to_string(y % 10) + "\n";
    }
    s += "  ";
    for (int x = 0; x < field.getWidth(); ++x) s += std::to_string(x % 10) + " ";
    s += "\n";
    std::cout << s;

    std::cout << "Здоровье: " << (int)player.getHealth() << "/" << player.getMaxHealth()
              << " | Урон: " << (int)player.attack()
              << " | Монеты: " << player.get_coins()
              << " | Оружие: " << (player.is_far_mode() ? "Копьё" : "Меч") << "\n";
}

void ConsoleRenderer::print_spells(const std::vector<std::string>& spells) {
    std::cout << "\n--- Ваши заклинания ---" << std::endl;
    if (spells.empty()) std::cout << "Пусто" << std::endl;
    for(const auto& s : spells) std::cout << s << std::endl;
    std::cout << "-----------------------\n" << std::endl;
}

void ConsoleRenderer::print_shop(const std::vector<std::string>& items) {
    std::cout << "\n--- Магазин заклинаний ---" << std::endl;
    if (items.empty()) std::cout << "В магазине нет товаров" << std::endl;
    for(const auto& s : items) std::cout << s << std::endl;
    std::cout << "--------------------------\n" << std::endl;
}
