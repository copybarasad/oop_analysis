#include "entity.hpp"

Entity::Entity(int hp, int damage, Fraction fraction) : hp(validate_hp(hp)),
    damage(validate_damage(damage)), fraction(fraction) {};

int Entity::validate_hp(int hp){
    if (hp > 1000 || hp < 0){
        hp = BASE_HP;
    }
    return hp;
}

int Entity::validate_damage(int damage){
    if (damage > 1000 || damage < 0){
        damage = BASE_DAMAGE;
    }
    return damage;
}

bool Entity::is_alive() {return hp > 0;};

int Entity::get_hp() {return hp;};
void Entity::set_hp(int hp) {this->hp = validate_hp(hp);};

int Entity::get_max_hp() {return max_hp;};
void Entity::set_max_hp(int max_hp) {this->max_hp = max_hp;};

int Entity::attack(Entity& target){
    int new_target_hp = target.hp - this->get_damage();
    if (new_target_hp < 0){
        new_target_hp = 0;
    }
    std::cout << "old hp: " << target.hp << "\tnew hp: " << new_target_hp << std::endl;
    target.set_hp(new_target_hp);

    return new_target_hp;
}

Fraction Entity::get_fraction() {return fraction;};

int Entity::get_base_damage() {return damage;};
void Entity::set_base_damage(int damage) {this->damage = validate_damage(damage);};
int Entity::get_damage(){return get_base_damage();};

int Entity::get_start_base_damage() {return start_damage;};
void Entity::set_start_base_demage(int damage) {start_damage = damage;};