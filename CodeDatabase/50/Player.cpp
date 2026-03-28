#include "Player.h"

 
Player::Player( double playerHealth, double playerDamage, double playerRangeDamage, int playerMana, double playerExp, int pos_x, int pos_y)
    : GameEnt(playerHealth,playerDamage,pos_x,pos_y, GameEntType::Player), 
      rangeDamage(playerRangeDamage),
      exp(playerExp),
      mana(playerMana),
      rangedAttack(false),  
      rangedAttackRange(2),
      maxHealth(playerHealth),
      maxMana(playerMana) {}


double Player::getPlayerDamage() const {
    return rangedAttack ? rangeDamage : getMeleeDamage();
}


void Player::reduceMana(int amount){
    mana = mana - amount;
}

//LVLUP
void Player::increaseMaxHealth(double amount) {
    maxHealth += amount;
    if (maxHealth < 1.0) maxHealth = 1.0;
    health = maxHealth;
}

void Player::increaseMeleeDamage(double amount) {
    damage += amount;
}

void Player::increaseRangeDamage(double amount) {
    rangeDamage += amount;
}

void Player::increaseMaxMana(int amount) {
    maxMana += amount;
    if (maxMana < 1) maxMana = 1;
    mana = maxMana;
}


//save and load
void Player::save(std::ostream& out) const {
    out << health << ' '
        << maxHealth << ' '
        << damage << ' '
        << rangeDamage << ' '
        << mana << ' '
        << maxMana << ' '
        << exp << ' '
        << pos_x << ' '
        << pos_y << ' '
        << rangedAttack << '\n';
}

void Player::load(std::istream& in) {
    in >> health >> maxHealth >> damage >> rangeDamage
       >> mana >> maxMana >> exp >> pos_x >> pos_y >> rangedAttack;
    if (!in) {
        throw InvalidSaveDataException("Ошибка при чтении данных игрока");
    }
}