#include "Player.h"
#include <iostream>
#include <iomanip>
using namespace std;


Player::Player(GameConfig& cfg) : Diver(cfg), level(cfg.pl_level), score(cfg.pl_score),
                                 currentAttackType(common_attack),
                                 currentSpellType(SpellFlag::none),
                                 commonDamage(cfg.common_damage),
                                 rangedDamage(cfg.ranged_damage),
                                 rangedRange(cfg.ranged_range),
                                 canAttack(true), bag(cfg){
    damage = commonDamage;
}

int Player::getLevel() {
    return level;
}

int Player::getScore() {
    return score;
}

AttackType Player::getAttackType() const {
    return currentAttackType;
}

string Player::getAttackTypeString() const {
    return (currentAttackType == common_attack) ? "Ближний бой" : "Дальний бой";
}

int Player::getRangedRange() const {
    return rangedRange;
}

bool Player::canAttackThisTurn() const {
    return canAttack;
}

void Player::resetAttack() {
    canAttack = true;
}

void Player::switchAttackType() {
    if(currentAttackType == common_attack) {
        currentAttackType = ranged_attack;
        damage = rangedDamage;
    } else {
        currentAttackType = common_attack;
        damage = commonDamage;
    }
    canAttack = false;
}

void Player::performAttack() {
    canAttack = false;
}

void Player::addLevel() {
    level += config.boost_level;
    commonDamage += config.boost_damage;
    rangedDamage += config.boost_damage;
    if(currentAttackType == common_attack) {
        damage = commonDamage;
    } else {
        damage = rangedDamage;
    }
}

void Player::addScore() {
    score += config.boost_score;
    if(score % 50 == 0){
        addLevel();
    }
}

Divers_bag& Player::getBag() {
    return bag;
}

bool Player::spendScoreOnSpell(int amount){
    if(amount >= score){
        return false;
    }else{
        score -= amount;
        return true;
    }
}

void Player::displayBag(){
    bag.display_bag();
}

void Player::getInfo() {
    cout << "______________________\n" << "Информация об игроке:\n";
    cout << setw(35) << left << "Здоровье:" << setw(30) << left << hp << endl;
    cout << setw(34) << left << "Уровень:" << setw(30) << left << level << endl;
    cout << setw(35) << left << "Тип атаки:" << setw(30) << left << getAttackTypeString() << endl;
    cout << setw(31) << left << "Урон:" << setw(30) << left << damage << endl;
    if (currentAttackType == ranged_attack) {
        cout << setw(36) << left << "Дальность:" << setw(30) << left << rangedRange << endl;
    }
    cout << setw(31) << left << "Опыт:" << setw(30) << left << score << endl;
}

void Player::reset(){
    hp = config.health;
    resetAttack();
    currentAttackType = common_attack;
    bag.reset();
}

void Player::saveTo(SaveFileRAII& out) const {
    Diver::saveTo(out);
    out.write(level).write(score);
    out.write(static_cast<int>(currentAttackType));
    out.write(static_cast<int>(currentSpellType));
    out.write(commonDamage).write(rangedDamage).write(rangedRange);
    out.write(canAttack);
    bag.saveTo(out);
}

void Player::loadFrom(SaveFileRAII& in) {
    Diver::loadFrom(in);
    in.read(level).read(score);
    in.read(currentAttackType);
    in.read(currentSpellType);
    in.read(commonDamage).read(rangedDamage).read(rangedRange);
    in.read(canAttack);
    bag.loadFrom(in);
}