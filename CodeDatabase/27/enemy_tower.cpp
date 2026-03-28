#include "enemy_tower.h"
#include "player.h"
#include "grid.h"
#include "point.h"


EnemyTower::EnemyTower(Grid& grid, int x, int y, int radius, int damage)
    : Unit(&grid, DEFAULT_HP, DEFAULT_DAMAGE, false, x, y),
      derezzPulse("Derezz Pulse", radius, damage),
      attacked(false) {
    Point* p = grid.getPoint(x, y);
    if (p) {
        current_point = p;
        p->set_occupier(this);
    }
}

void EnemyTower::on_move_to_point(Point& /*point*/) {
}

bool EnemyTower::try_attack(Player& player) {
    if (!grid) return false;               
    return derezzPulse.tower_attack(*this, *grid, player);
}