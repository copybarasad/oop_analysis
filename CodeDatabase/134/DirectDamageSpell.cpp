#include "DirectDamageSpell.h"
#include "GameField.h"
#include "Enemy.h"

DirectDamageSpell::DirectDamageSpell(int damage, int range)
    : damage(damage), range(range) {}

void DirectDamageSpell::Use(Player* player, GameField* field, int targetX, int targetY) {
    if (!field->IsInside(targetX, targetY)) return;

    for (Enemy* enemy : field->GetEnemies()) {
        int ex = enemy->GetX();
        int ey = enemy->GetY();
        int dx = ex - targetX;
        int dy = ey - targetY;
        if (dx*dx + dy*dy <= range*range) {
            enemy->TakeDamage(damage);
            break; 
        }
    }
}
