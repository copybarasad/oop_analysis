#include "game_renderer.h"
#include "playing_field.h"
#include "player.h"
#include <iostream>
#include <iomanip>

void ConsoleRenderer::showPlayerStats(const Player& player, const PlayingField& field) const {
    auto player_coords = field.getBattlerCoords(const_cast<Player*>(&player));
    std::cout << "\n---------------------------------" << std::endl;
    std::cout << "---          ХОД Игрока       ---" << std::endl;
    std::cout << "---------------------------------" << std::endl;
    std::cout << "Статус: " << player.getName()
        << " | Координаты: (" << player_coords.first << "," << player_coords.second << ")"
        << " | Здоровье: " << player.getHealth()
        << " | Счет: " << player.getScore() << std::endl;
    std::cout << "Действия: (w,a,s,d) - движение; c - заклинание; b - магазин; p - сохранить игру, q - выход: ";
}

void ConsoleRenderer::draw(const PlayingField& field) const {
    std::cout << "\n";
    std::cout << "   ";
    for (int x = 0; x < field.getWidth(); x++) {
        std::cout << std::setw(2) << x << " ";
    }
    std::cout << "\n";

    std::cout << "   ";
    for (int x = 0; x < field.getWidth(); x++) {
        std::cout << "---";
    }
    std::cout << "-\n";

    for (int y = field.getHeight() - 1; y >= 0; y--) {
        std::cout << std::setw(2) << y << "|";
        for (int x = 0; x < field.getWidth(); x++) {
            BasicBattler* battler = field.getBattlerAt(x, y);
            if (battler == nullptr) {
                std::cout << " . ";
            }
            else {
                if (battler->getFaction() == Faction::PLAYER) {
                    std::cout << " P ";
                }
                else {
                    std::cout << " E ";
                }
            }
        }
        std::cout << "|\n";
    }

    std::cout << "   ";
    for (int x = 0; x < field.getWidth(); x++) {
        std::cout << "---";
    }
    std::cout << "-\n";
    std::cout << "Легенда: P - Игрок, E - Враг, . - Пустая клетка\n";
}