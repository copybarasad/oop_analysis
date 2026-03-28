#include "Character.hpp"
#include <iostream> 

Character::Character(int hp, int damage, Position pos, Logger& logger)
    : hp_(hp),               
      damage_(damage),   
      pos_(pos),              
      attack(attackType::melee), 
      logger_(logger)         
{
    if (hp < 0) 
        throw std::invalid_argument("hp should be > 0!\n");
    if (damage < 0) 
        throw std::invalid_argument("damage should be > 0!\n");
    if (pos.x < 0 || pos.y < 0) 
        throw std::invalid_argument("invalid coords\n");
}

bool Character::isAlive() const { return hp_ > 0; }

void Character::takeDamage(int damage) {
    hp_ -= damage;
    if (hp_ < 0) hp_ = 0;
    logger_.info("HP left: " + std::to_string(hp_));
}

int Character::distanceTo(const Character& other) const {
    int dx = std::abs(pos_.x - other.getPos().x);
    int dy = std::abs(pos_.y - other.getPos().y);
    return std::max(dx, dy);
}

void Character::melee_attack(Character& other) { other.takeDamage(damage_); }
void Character::range_attack(Character& other) { other.takeDamage(damage_ / 2); }

void Character::setPos(Position pos) { pos_ = pos; }
int Character::getHp() const { return hp_; }
int Character::getDamage() const { return damage_; }
Position Character::getPos() const { return pos_; }
attackType Character::getAttackType() const { return attack; }



void Character::save(std::ostream& os) const {
    os << hp_ << " " << damage_ << " " 
       << pos_.x << " " << pos_.y << " "
       << static_cast<int>(attack) << "\n";
}

void Character::load(std::istream& is) {
    int atkVal;
    is >> hp_ >> damage_ >> pos_.x >> pos_.y >> atkVal;
    attack = (atkVal == 1) ? attackType::range : attackType::melee;
}
