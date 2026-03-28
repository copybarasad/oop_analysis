#include "EnemyTower.h"
#include "Constants.h"
#include "GameField.h"
#include "Player.h"
#include "Spell.h"
#include <cmath>

Spell* makeDirectSpell(int damage, int range);

EnemyTower::EnemyTower(int x, int y, int hp, int range, int damage, int cooldown)
    : Entity(x, y, hp), range(range), damage(damage), cooldownRemaining(cooldown) {}

void EnemyTower::tickAndAttack(GameField& field, Player& player) {
    if (!isAlive()) return;
    if (cooldownRemaining > 0) { cooldownRemaining--; return; }
    
    int dx = std::abs(player.getX() - getX());
    int dy = std::abs(player.getY() - getY());
    int dist = dx + dy;
    if (dist > range) return;
    
    Spell* spell = makeDirectSpell(damage, range);
    bool attacked = spell->cast(field, player, player.getX(), player.getY());
    delete spell;
    
    if (attacked) {
        cooldownRemaining = TOWER_COOLDOWN;
    }
}


