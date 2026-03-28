#include "spell.h"
#include "player.h"
#include "field.h"
#include <cmath> 

bool DirectDamageSpell::Use(Player& player, Field& field, int targetX, int targetY) {
    player.ApplyBuffs();
    player.ResetBuffs();

    int playerX = field.GetPlayerX();
    int playerY = field.GetPlayerY();
    double distance = std::sqrt(std::pow(targetX - playerX, 2) + std::pow(targetY - playerY, 2));
    if (distance > 2 + player.GetBuffRange()) {
        return false; 
    }

    if (field.GetEnemyAt(targetX, targetY)) {
        field.GetEnemyAt(targetX, targetY)->TakeDamage(20 + player.GetBuffRange());
        return true;
    } else if (field.GetBuildingAt(targetX, targetY)) {
        field.GetBuildingAt(targetX, targetY)->TakeDamage(20 + player.GetBuffRange());
        return true;
    } else if (field.GetTowerAt(targetX, targetY)) {
        field.GetTowerAt(targetX, targetY)->TakeDamage(20 + player.GetBuffRange());
        return true;
    }
    return false;
}


bool AreaDamageSpell::Use(Player& player, Field& field, int targetX, int targetY) {
    player.ApplyBuffs();
    player.ResetBuffs();

    int size = 2 + player.GetBuffArea();
    int startX = targetX - size / 2;
    int startY = targetY - size / 2;
    for (int dx = 0; dx < size; ++dx) {
        for (int dy = 0; dy < size; ++dy) {
            int x = startX + dx;
            int y = startY + dy;
            if (field.IsValidPosition(x, y)) {  
                if (auto enemy = field.GetEnemyAt(x, y)) {
                    enemy->TakeDamage(20);
                }
            }
        }
    }
    return true; 
}

bool TrapSpell::Use(Player& player, Field& field, int targetX, int targetY) {
    player.ApplyBuffs();
    player.ResetBuffs();

    if (field.IsEmpty(targetX, targetY)) {
        field.AddTrap(targetX, targetY, 10 + player.GetBuffTrapDamage()); 
        return true;
    }
    return false;
}

bool SummonSpell::Use(Player& player, Field& field, int targetX, int targetY) {
    player.ApplyBuffs();
    player.ResetBuffs();

    int summons = 1 + player.GetBuffSummons();
    for (int i = 0; i < summons; ++i) {
        int allyX = targetX + i;
        int allyY = targetY;
        if (field.IsValidPosition(allyX, allyY) && field.IsEmpty(allyX, allyY)) { 
            field.AddAlly(allyX, allyY);
        }
    }
    return true;
}

bool BuffSpell::Use(Player& player, Field& field, int targetX, int targetY) {
    player.AddBuff();
    return true;
}

