#pragma once

#include "objects/Enemy.h"
#include "Field.h"
#include "objects/GameObject.h"

enum class AttackType {
    RANGE,
    SPELL,
    NONE,
};

class AttackSystem {
    Field *field;
    Player *player;
    AttackType attackType = AttackType::NONE;
    int activeSpellIndex = -1;

public:
    AttackSystem(Field* _field, Player* _player)
        : field(_field), player(_player) {}
    
    void updateReferences(Field* _field, Player* _player) {
        field = _field;
        player = _player;
    }
    
    void takeDamageToCell(int dmg, int cellX, int cellY) {
        if (!field->isInBounds(cellX, cellY)) return;
        GameObject* object = field->getObjectAt(cellX, cellY);
        if (object && object->isEnemy()) {
            Enemy* enemy = static_cast<Enemy*>(object);
            enemy->takeDamage(dmg);
        }
    }

    void takeDamageToArea(int dmg, int area, int centerX, int centerY) {
        for (auto& obj : field->getObjects()) {
            if (!obj->isEnemy()) continue;
            int x = obj->getX();
            int y = obj->getY();
            if (x >= centerX - area && x <= centerX + area 
                    && y >= centerY - area && y <= centerY + area) {
                Enemy* enemy = static_cast<Enemy*>(obj.get());
                enemy->takeDamage(dmg);
            }
        }
    }

    bool executeAttack(int targetX, int targetY) {
        switch (attackType) {
            case AttackType::RANGE: {
                int dmg = player->getRangeDamage();
                if (player->isInAttackRange(targetX, targetY)) {
                    takeDamageToCell(dmg, targetX, targetY);
                    return true;
                }
                return false;
            }
            case AttackType::SPELL: {
                Hand& hand = player->getHand();
                if (activeSpellIndex == -1) return false;
                bool success = hand.useSpell(activeSpellIndex, *field, player, targetX, targetY);

                if (!success) return false;

                activeSpellIndex = -1;
                return true;
            }
            case AttackType::NONE:
            default:
                return false;
        }
    }

    AttackType getAttackType() { return attackType; }
    int getActiveSpellIndex() { return activeSpellIndex; }
    
    void setAttackType(AttackType type) { attackType = type; }
    void setActiveSpellIndex(int spellIdnex) { activeSpellIndex = spellIdnex; }
};
