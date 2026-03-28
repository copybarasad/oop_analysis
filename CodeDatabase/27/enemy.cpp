#include "enemy.h"
#include "player.h"
Enemy::Enemy(Grid& grid, int x, int y) 
    : Unit(&grid, DEFAULT_HP / 2, DEFAULT_DAMAGE, false, x, y) { 
    
    Point* start_point = grid.getPoint(x, y);
    if (start_point) {
        current_point = start_point;
        start_point->set_occupier(this);
    }
}

void Enemy::on_move_to_point(Point& point) {
    if (point.has_trap()) { 
        int dmg = point.pop_trap_damage();
        this->take_damage(dmg);
        if (this->get_hp() <= 0) {
            return;
        }
    }
    if (point.is_occupied() && point.get_occupier() != nullptr) {
        if (Player* player = dynamic_cast<Player*>(point.get_occupier())) {
            player->take_damage(this->damage);
        }
    } else {
        point.set_occupier(this);
    }
}