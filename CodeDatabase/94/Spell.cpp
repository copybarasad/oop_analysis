#include "Spell.h"
#include "HealSpell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "Player.h"
#include "Enemy.h"
#include <cmath>

bool HealSpell::cast(const Position& target, GameField& field, Character& caster) {
    Player* player = dynamic_cast<Player*>(&caster);
    if (!player) {
        return false;
    }
    
    int oldHealth = player->getHealth();
    player->heal(20);
    return true;
}

bool DirectDamageSpell::cast(const Position& target, GameField& field, Character& caster) {
    Player* player = dynamic_cast<Player*>(&caster);
    if (!player) return false;
    
    Position casterPos = player->getPosition();
    
    int dx = target.x() - casterPos.x();
    int dy = target.y() - casterPos.y();
    
    if (dx != 0) dx = dx / abs(dx);
    if (dy != 0) dy = dy / abs(dy);
    
    for (int distance = 1; distance <= range; ++distance) {
        Position checkPos(casterPos.x() + dx * distance, casterPos.y() + dy * distance);
        
        if (!field.isValidPosition(checkPos)) {
            break;
        }
        
        if (!field.isPositionPassable(checkPos)) {
            break;
        }
        
        if (field.hasCharacterAt(checkPos)) {
            const Character* character = field.viewCharacterAt(checkPos);
            if (character && dynamic_cast<const Enemy*>(character)) {
                Character* enemy = const_cast<Character*>(character);
                enemy->takeDamage(damage);
                
                if (!enemy->isAlive()) {
                    field.removeCharacter(checkPos);
                    player->addScore(10);
                }
                return true;
            }
        }
    }
    
    return false;
}

bool AreaDamageSpell::cast(const Position& target, GameField& field, Character& caster) {
    Player* player = dynamic_cast<Player*>(&caster);
    if (!player) return false;
    
    Position casterPos = player->getPosition();
    
    int dx = target.x() - casterPos.x();
    int dy = target.y() - casterPos.y();
    
    if (dx != 0) dx = dx / abs(dx);
    if (dy != 0) dy = dy / abs(dy);
    
    Position finalTarget = target;
    
    for (int distance = 1; distance <= range; ++distance) {
        Position checkPos(casterPos.x() + dx * distance, casterPos.y() + dy * distance);
        
        if (!field.isValidPosition(checkPos)) {
            break;
        }
        
        if (!field.isPositionPassable(checkPos)) {
            break;
        }
        
        finalTarget = checkPos;
    }
    
    bool hitSomething = false;
    int enemiesKilled = 0;
    
    for (int y = finalTarget.y(); y <= finalTarget.y() + 1; ++y) {
        for (int x = finalTarget.x(); x <= finalTarget.x() + 1; ++x) {
            Position areaPos(x, y);
            if (field.isValidPosition(areaPos) && field.hasCharacterAt(areaPos)) {
                Character* character = field.getCharacterAt(areaPos);
                if (character && dynamic_cast<Enemy*>(character)) {
                    Enemy* enemy = dynamic_cast<Enemy*>(character);
                    enemy->takeDamage(damage);
                    
                    if (!enemy->isAlive()) {
                        field.removeCharacter(areaPos);
                        enemiesKilled++;
                        hitSomething = true;
                    }
                }
            }
        }
    }
    
    if (enemiesKilled > 0) {
        player->addScore(enemiesKilled * 10);
    }
    
    return true;
}