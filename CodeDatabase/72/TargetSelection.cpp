//
// Created by Mac on 25.10.2025.
//

#include "TargetSelection.h"

#include <iostream>

TargetSelection::TargetSelection(Field &field)
    :field(field) {}

Position TargetSelection::selectArea(int areaWidth, int areaHeight, int radius) const {
    Position cursor = field.getPlayer()->getPosition();
    Position playerPos = field.getPlayer()->getPosition();

    while (true) {
        system("clear");
        int width = field.getWidth();
        int height = field.getHeight();

        std::cout << "┌";
        for (int i = 0; i < width * 2; i++) std::cout << "─";
        std::cout << "┐\n";

        for (int y = 0; y < height; y++) {
            std::cout << "│";
            for (int x = 0; x < width; x++) {
                Position pos{x, y};
                bool drawn = false;

                // проверяем, входит ли клетка в зону применения (радиус)
                int dx = x - playerPos.getX();
                int dy = y - playerPos.getY();
                double distance = sqrt(dx * dx + dy * dy);
                bool inRadius = distance <= radius;

                bool inArea =
                    x >= cursor.getX() && x < cursor.getX() + areaWidth &&
                    y >= cursor.getY() && y < cursor.getY() + areaHeight;

                if (inRadius && !inArea)
                    std::cout << "\033[46m";
                else if (inArea)
                    std::cout << "\033[44m";

                for (const auto &enemy : field.getEnemies()) {
                    if (pos == enemy.getPosition()) {
                        std::cout << "\033[31mE\033[0m ";
                        drawn = true;
                        break;
                    }
                }

                if (!drawn) {
                    for (const auto &enemy_build : field.getEnemiesBuild()) {
                        if (pos == enemy_build.getPosition()) {
                            std::cout << "\033[31mB\033[0m ";
                            drawn = true;
                            break;
                        }
                    }
                }

                if (!drawn) {
                    for (const auto &tower : field.getEnemyTowers()) {
                        if (pos == tower.getPosition()) {
                            std::cout << "\033[31mI\033[0m ";
                            drawn = true;
                            break;
                        }
                    }
                }

                if (!drawn) {
                    if (pos == playerPos) {
                        std::cout << "\033[32mP\033[0m "; // игрок зелёным
                    } else {
                        switch (field.getCell(pos).getState()) {
                            case State::Free: std::cout << ". "; break;
                            case State::Obstacle: std::cout << "O "; break;
                            case State::Slow: std::cout << "~ "; break;
                        }
                    }
                }

                std::cout << "\033[0m";
            }
            std::cout << "│\n";
        }

        std::cout << "└";
        for (int i = 0; i < width * 2; i++) std::cout << "─";
        std::cout << "┘\n";

        std::cout << "Use [WASD] to move area, [F] to confirm, [Q] to cancel.\n";
        std::cout << "Radius limit: " << radius << "\n";

        char op;
        std::cin >> op;
        op = tolower(op);

        Position newCursor = cursor;
        switch (op) {
            case 'w': if (cursor.getY() > 0) newCursor.setY(cursor.getY() - 1); break;
            case 's': if (cursor.getY() + areaHeight < height) newCursor.setY(cursor.getY() + 1); break;
            case 'a': if (cursor.getX() > 0) newCursor.setX(cursor.getX() - 1); break;
            case 'd': if (cursor.getX() + areaWidth < width) newCursor.setX(cursor.getX() + 1); break;
            case 'f': return cursor;
            case 'q': return {-1, -1};
        }

        int dxNew = newCursor.getX() - playerPos.getX();
        int dyNew = newCursor.getY() - playerPos.getY();
        double newDistance = sqrt(dxNew * dxNew + dyNew * dyNew);

        if (newDistance <= radius) {
            cursor = newCursor;
        }
    }
}



