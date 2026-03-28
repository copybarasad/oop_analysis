#include "AreaDamageSpell.h"
#include "GameField.h"
#include "Enemy.h"

AreaDamageSpell::AreaDamageSpell(int damage, int range)
    : damage(damage), range(range) {}

void AreaDamageSpell::Use(Player* player, GameField* field, int targetX, int targetY) {
    if (!field->IsInside(targetX, targetY)) return;

    for (int dx = 0; dx < 2; ++dx) {
        for (int dy = 0; dy < 2; ++dy) {
            int x = targetX + dx;
            int y = targetY + dy;
            if (field->IsInside(x, y)) {
                for (Enemy* enemy : field->GetEnemies()) {
                    if (enemy->GetX() == x && enemy->GetY() == y) {
                        enemy->TakeDamage(damage);
                    }
                }
            }
        }
    }
}
