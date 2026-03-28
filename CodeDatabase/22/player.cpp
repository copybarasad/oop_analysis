#include "characters/player.h"
#include <iostream> 
#include "characters/enemy.h"  
#include "spells/hand.h"   
#include "field/position.h"

#define STARTPOS Position {1, 1}


// Constructor/Destructor
Player::Player(int health, int damage, Position position):Character(health, damage, position), level_(1), score_(0), weapon_("knight"){
    hand_ = std::make_unique<Hand>(10);
}

Player::Player() : Character(100, 20, STARTPOS), level_(1), score_(0), weapon_("knight")
{
    hand_ = std::make_unique<Hand>(10);
}

// Active methods
void Player::attack(Enemy& enemy)
{
    enemy.takeDamage(this->getDamage());
}

void Player::changeWeapon(){
    this->weapon_ = (weapon_ == "knight") ? "gun" : "knight";
    this->setDamage((weapon_ == "knight")? this->getDamage() : 2*this->getDamage());
}

void Player::levelUp(){
    if (this->score_ == 100){
        this->level_ += 1;
        this->score_ = 0;
    }
    
}

void Player::scoreUp(){
    this->score_+=10;
}


void Player::setWeapon(std::string weapon){this->weapon_ = weapon;}
void Player::setLevel(int lvl){this->level_ = lvl;}
void Player::setScore(int score){this->score_ = score;}

// getters
int Player::getLevel() const {return level_;}
int Player::getScore() const {return score_;}
std::string Player::getWeapon() const {return weapon_;}
Hand* Player::getHand(){return hand_.get();}
