#include "enemy.h"
#include "weapon.h"

static Weapon enemy_weapon("Sword", 10, WeaponType::Melee);

Enemy::Enemy(int health, int x, int y)
    : Entity(health, &enemy_weapon, x, y) {}

Enemy::Enemy(int health, int x, int y, int damage)
    : Entity(health, nullptr, x, y) {
    static Weapon custom_weapon("Sword", damage, WeaponType::Melee);
    set_weapon(&custom_weapon);
}

void Enemy::save(std::ostream& os) const {
    Entity::save(os);
    os << " ";
}

void Enemy::load(std::istream& is) {
    Entity::load(is);
    if (get_weapon() == nullptr) {
        set_weapon(&enemy_weapon);
    }
}