#include "Character.h"
#include <algorithm>

Character::Character() : hp_(0), damage_(0), pos_(Coord{0,0}) {}

Character::Character(int hp, int damage, Coord pos)
    : hp_(hp), damage_(damage), pos_(pos) {}

int Character::HP() const { return hp_; }
void Character::SetHP(int hp) { hp_ = hp; }
int Character::Damage() const { return damage_; }

Coord Character::Pos() const { return pos_; }
void Character::SetPos(Coord p) { pos_ = p; }

bool Character::IsAlive() const { return hp_ > 0; }

void Character::TakeDamage(int dmg) {
    if (dmg <= 0) return;
    hp_ = hp_ - dmg;
}
