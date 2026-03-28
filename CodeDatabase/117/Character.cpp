#include "Character.h"
#include "algorithm"

Character::Character(int maxHealth, CharacterCondition condition):
    condition(condition), maxHealth(maxHealth), health(maxHealth){
    this->position = std::make_pair(0, 0);
}

Character::Character(): Character(DEFAULT_MAX_HEALTH, CharacterCondition::Normal) {}

void Character::setHP(int health){
    this->health = std::clamp(health, 0, this->maxHealth);
}

int Character::getHP() const{
    return this->health;
}

void Character::setMaxHP(int maxHealth){
    this->maxHealth = std::max(1, maxHealth);
    this->health = std::min(this->health, this->maxHealth);
}

int Character::getMaxHP() const{
    return this->maxHealth;
}

void Character::setCondition(CharacterCondition condition){
    this->condition = condition;
}

CharacterCondition Character::getCondition() const{
    return this->condition;
}

void Character::takeDMG(int damage){
    this->health = std::clamp(this->health - damage, 0, this->maxHealth);
}

bool Character::isAlive() const{
    return (this->health > 0);
}

std::pair<int, int> Character::getPos() const{
    return this->position;
}

void Character::setPos(std::pair<int, int> position){
    this->position = position;
}

void Character::setPos(int x, int y){
    this->setPos(std::make_pair(x, y));
}

int Character::squareDistTo(Character target) const{
    int myX = this->position.first;
    int myY = this->position.second;
    int tarX = target.getPos().first;
    int tarY = target.getPos().second;
    int diffX = myX - tarX;
    int diffY = myY - tarY;
    return diffX * diffX + diffY * diffY;
}

int Character::squareDistTo(std::pair<int, int> target) const{
    int myX = this->position.first;
    int myY = this->position.second;
    int tarX = target.first;
    int tarY = target.second;
    int diffX = myX - tarX;
    int diffY = myY - tarY;
    return diffX * diffX + diffY * diffY;
}

std::string Character::getName(){
    return "Character";
}