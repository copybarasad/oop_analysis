#include "ActorAttack.hpp"


Attack::Attack() : Attack(DEFAULT_ATTACK) {}


Attack::Attack(int attack) {
    this->attack = attack;
}


Attack::~Attack() {
}


int Attack::getAttack() const {
    return attack < 0 ? 0 : attack;
}


void Attack::setAttack(int attack) {
    this->attack = attack;
}


AttackSaveData Attack::getAttackSavedata() {
    AttackSaveData data;
    data.attack = this->attack;
    return data;
}

void Attack::setAttackSaveData(AttackSaveData data) {
    this->attack = data.attack;
}