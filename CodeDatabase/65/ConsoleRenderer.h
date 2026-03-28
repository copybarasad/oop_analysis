#pragma once

#include <iostream>
#include "Colors.h"
#include "Field.h"
#include "systems/TargetingSystem.h"
#include "objects/Player.h"

class ConsoleRenderer {
protected:
    Field* field;
    Player* player;
    TargettingSystem& targettingSystem;

    const int cellSizeX = 2;
    const int cellSizeY = 1;

public:
    ConsoleRenderer(Field* _field, Player* _player, TargettingSystem& _targettingSystem)
        : field(_field), player(_player), targettingSystem(_targettingSystem) {}

    virtual ~ConsoleRenderer() = default;

    void updateReferences(Field* _field, Player* _player) {
        field = _field;
        player = _player;
    }

    void moveCursor(int x, int y) {
        std::cout << "\033[" << y << ";" << x << "H";
    }

    void clearScreen() {
        std::cout << "\033[2J\033[H";
    }

    virtual void render() {
        clearScreen();
        drawGrid();
        drawObjects();
        drawPlayer();
        drawCursor();
    }

protected:
    virtual void drawGrid() {
        if (!field) return;
        
        int width = field->getWidth();
        int height = field->getHeight();
        
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                bool inRange = false;
                bool inTargetArea = targettingSystem.isInTargetArea(i, j);

                if (targettingSystem.getIsActive()) {
                    Player* p = targettingSystem.getActivePlayer();
                    ISpell* spell = targettingSystem.getActiveSpell();

                    if (spell) {
                        inRange = spell->isInRange(p->getX(), p->getY(), i, j);
                    } else if (p) {
                        inRange = player->isInAttackRange(i, j);
                    }
                }

                if (inTargetArea) {
                    std::cout << "\033[41m";
                } else if (inRange) {
                    std::cout << "\033[44m";
                }

                std::cout << TermColors::DARK_MAGENTA;
                std::cout << ".";
                std::cout << "\033[0m" << " ";
            }
            std::cout << '\n';
        }
        std::cout << "\033[0m";
    }

    virtual void drawObjects() {
        if (!field) return;
        
        for (auto& object : field->getObjects()) {
            int screenX = cellSizeX * object->getX() + 1;
            int screenY = cellSizeY * object->getY() + 1;
            moveCursor(screenX, screenY);
            std::cout << object->getColor() << object->getSprite() << "\033[0m";
            std::cout << std::endl;
        }
    }

    virtual void drawPlayer() {
        if (!field || !player) return;
        
        Player* p = field->getPlayer();
        if (!p) return;

        int screenX = cellSizeX * p->getX() + 1;
        int screenY = cellSizeY * p->getY() + 1;

        moveCursor(screenX, screenY);
        std::cout << p->getColor() << p->getSprite() << "\033[0m";
        std::cout << std::endl;
    }

    virtual void drawCursor() {
        if (!targettingSystem.getIsActive()) return;

        int cursorX = targettingSystem.getCursorX();
        int cursorY = targettingSystem.getCursorY();

        bool canUse = false;
        Player *p = targettingSystem.getActivePlayer();
        ISpell* spell = targettingSystem.getActiveSpell();

        if (spell) {
            canUse = spell->isInRange(p->getX(), p->getY(), cursorX, cursorY);
        } else if (p) {
            canUse = player->isInAttackRange(cursorX, cursorY);
        }

        if (targettingSystem.getTargettingMode() == TargettingMode::AREA_ATTACK && spell) {
            int areaSize = spell->getAreaSize();

            for (int dy = 0; dy < areaSize; dy++) {
                for (int dx = 0; dx < areaSize; dx++) {
                    int x = cursorX + dx;
                    int y = cursorY + dy;

                    if (!field->isInBounds(x, y)) continue;

                    int cellX = cellSizeX * x + 1;
                    int cellY = cellSizeY * y + 1;

                    moveCursor(cellX, cellY);

                    if (canUse) {
                        std::cout << "\033[43m"; // YELLOW
                    } else {
                        std::cout << "\033[41m"; // RED
                    }

                    std::cout << "  ";
                    std::cout << "\033[0m";
                }
            }
        } else {
            int cellX = cellSizeX * cursorX + 1;
            int cellY = cellSizeY * cursorY + 1;

            moveCursor(cellX, cellY);

            if (canUse) {
                std::cout << "\033[32m"; // GREEN
            } else {
                std::cout << "\033[31m"; // RED
            }

            std::cout << "X";
            std::cout << "\033[0m";
        }

        std::cout << "\033[0m" << std::endl;
    }
};
