#include "Player.h"
#include <iostream>

Player::Player(int playerId, const std::string& playerName, int initialHP, 
                CombatStyle style, int startX, int startY)
    : Character(playerId, playerName, initialHP, style, startX, startY), score(100), hand(SpellHand(3)) {}

int Player::getScore() const { return score; }
void Player::addScore(int points) { score += points; }
SpellHand& Player::getHand() { return hand; }
const SpellHand& Player::getHand() const { return hand; }

void Player::switchStyle(){
    if (combatStyle == CombatStyle::MELEE){
        combatStyle = CombatStyle::LR;
    } else {
        combatStyle = CombatStyle::MELEE;
    }
    std::cout << name << " switched to " 
    << (combatStyle == CombatStyle::MELEE ? "MELEE" : "LONG RANGED") 
    << " combat. Damage " << getDamage() << "\n";
}

void Player::move(int deltaX, int deltaY){
    if (!getisAlive()) return;
    posX += deltaX;
    posY += deltaY;
}

void Player::attack(Character& target){
    if (!getisAlive() || !target.getisAlive()) return;

    int damage = getDamage();
    std::cout << name << " attacks " << target.getName()
              << " with " << (combatStyle == CombatStyle::MELEE ? "MELEE" : "RANGED")
              << " style for " << damage << " damage!\n";
    target.takeDamage(damage);
}

bool Player::canMoveTo(int targetX, int targetY) const{
    return getisAlive();
}

void Player::save(std::ostream& out) const {
    Character::save(out);
    out.write(reinterpret_cast<const char*>(&score), sizeof(score));
    hand.save(out);
}

void Player::load(std::istream& in){
    Character::load(in);
    in.read(reinterpret_cast<char*>(&score), sizeof(score));
    hand.load(in);
}