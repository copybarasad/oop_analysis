#pragma once

#include "objects/Player.h"
#include "Field.h"
#include "interfaces/ISpell.h"

enum class TargettingMode {
    NONE,
    DIRECT_ATTACK,
    AREA_ATTACK,
};

class TargettingSystem {
    bool isActive;
    int cursorX;
    int cursorY;
    TargettingMode mode;
    ISpell* activeSpell;
    Player* activePlayer;

public:
    TargettingSystem() : isActive(false), cursorX(0), cursorY(0), mode(TargettingMode::NONE), activeSpell(nullptr), activePlayer(nullptr) {}
    bool getIsActive() { return isActive; }
    int getCursorX() { return cursorX; }
    int getCursorY() { return cursorY; }
    TargettingMode getTargettingMode() { return mode; }
    ISpell* getActiveSpell() { return activeSpell; }
    Player* getActivePlayer() { return activePlayer; }

    void startRangedAttack(Player* player) {
        isActive = true;
        mode = TargettingMode::DIRECT_ATTACK;
        activePlayer = player;
        activeSpell = nullptr;

        cursorX = player->getX();
        cursorY = player->getY();
    }

    void startCastSpell(Player* player, ISpell* spell) {
        if (!spell) return;

        activeSpell = spell;
        activePlayer = player;

        switch (spell->getType()) {
            case SpellType::DIRECT_DAMAGE:
                mode = TargettingMode::DIRECT_ATTACK;
                break;
            case SpellType::TRAP:
                mode = TargettingMode::DIRECT_ATTACK;
                break;
            case SpellType::AREA_DAMAGE:
                mode = TargettingMode::AREA_ATTACK;
                break;
            case SpellType::SPELL_TYPE_COUNT:
            default:
                return;
        }

        isActive = true;

        cursorX = player->getX();
        cursorY = player->getY();
    }

    void moveCursor(int dx, int dy, const Field& field) {
        if(!isActive) return;

        int newX = cursorX + dx;
        int newY = cursorY + dy;

        if (field.isInBounds(newX, newY)) {
            cursorX = newX;
            cursorY = newY;
        }
    }

    bool confirm() {
        if (!isActive) return false;
        isActive = false;
        return true;
    }

    void cancel() {
        isActive = false;
        mode = TargettingMode::NONE;
    }

    bool isInTargetArea(int x, int y) {
        if (!isActive) return false;

        if (mode == TargettingMode::AREA_ATTACK && activeSpell) {
            int areaSize = activeSpell->getAreaSize();
            return x >= cursorX && x < cursorX + areaSize
                && y >= cursorY && y < cursorY + areaSize;
        }

        return x == cursorX && y == cursorY;
    }
};
