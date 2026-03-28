#include "player.h"

Player::Player(int X, int Y, int hp, int dam, int point, int m, int max_HP) 
    : GameObject(X, Y, hp, "melee", 1, 0), damage(dam), Points(point), mana(m), hand(), max_hp(max_HP) {
} 

int Player::getDamage() {
    return damage;
}
void Player::addKill() {
    Points += 1;
    
    if (Points >= 5) {
        hand.addRandomSpell();
        Points = 0;
    }
}

int Player::getMaxHP(){
    return max_hp;
}

nlohmann::json Player::toJson() {
    return {
        {"x", getX()},
        {"y", getY()},
        {"health", getHP()},
        {"damage", damage},
        {"mana", mana},
        {"points", Points},
        {"combat_type", getTypeCombat()},
        {"max_hp", max_hp}
    };
}

void Player::upPlayer(){
    int bonus = random_numb(1,3);
    switch (bonus){
    case 1:
        damage += 4;
        break;
    case 2:
        mana += 10;
        break;
    case 3:
        HP += 10;
        max_hp += 10;
        break;
    }
}
void Player::setHP(int hp){
    HP = hp;
}
void Player::fromJson(const nlohmann::json& j) {
    x = j["x"];
    y = j["y"];
    HP = j["health"];
    damage = j["damage"];
    mana = j["mana"];
    Points = j["points"];
    type_combat = j["combat_type"];
    max_hp = j["max_hp"];
}

void Player::setDamage(int new_damage) {  
    damage = new_damage;
}

void Player::setHand(Hand&& hands){
    hand = std::move(hands);
}

void Player::setRange(int new_range) {  
    ranged = new_range;
}
int Player::getMana() { return mana; }
void Player::reduceMana(int amount) { mana -= amount; }
void Player::setMana(int new_mana) { mana = new_mana; }
Hand& Player::getHand() { return hand; }
int Player::getPoints() {
    return Points;
}