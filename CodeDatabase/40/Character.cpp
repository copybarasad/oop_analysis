#include "Character.h"
#include <iostream>
#include <stdexcept>

void Character::validateHP() const {
    if (max_hp <= 0){
        throw std::invalid_argument("Maximum hp must be positibe");
    }
}

int Character::calculateActualDamage() const {
    return (combatStyle == CombatStyle::MELEE) ? 5 : 3;
}

Character::Character(int characterId, const std::string& characterName, 
                   int initialHP, CombatStyle style, int startX, int startY)
    : name(characterName), max_hp(initialHP), 
      hp(initialHP), combatStyle(style), posX(startX), posY(startY), isAlive(true) 
{
    validateHP();
}

int Character::getId() const {return id;}
std::string Character::getName() const {return name;}
int Character::getHP() const {return hp;}
int Character::getMaxHP() const {return max_hp;}
CombatStyle Character::getCombatStyle() const {return combatStyle;};
int Character::getDamage() const {return calculateActualDamage();}
int Character::getPosX() const {return posX;}
int Character::getPosY() const {return posY;}
bool Character::getisAlive() const {return isAlive;}

void Character::setHP(int newHP){
    if (newHP < 0) { newHP = 0; }
    if (newHP > max_hp) { newHP = max_hp; }
    hp = newHP;
    isAlive = (hp > 0);
}

void Character::setCombatStyle(CombatStyle newStyle){
    combatStyle = newStyle;
}

void Character::setPos(int newX, int newY){
    posX = newX;
    posY = newY;
}

void Character::takeDamage(int damageAmount){
    if (damageAmount < 0) {
        throw std::invalid_argument("Damage amount cannot be negative");
    }
    hp -= damageAmount;
    if (hp <= 0){
        hp = 0;
        isAlive = false;
    }

}


void Character::heal(int healAmount){
    if (healAmount > 0 && isAlive){
        hp += healAmount;
        if (hp < max_hp){
            hp = max_hp;
        }
    }
}


void Character::save(std::ostream& out) const {
    out.write(reinterpret_cast<const char*> (&id), sizeof(id));
    size_t nameLen = name.length();
    out.write(reinterpret_cast<const char*> (&nameLen), sizeof(nameLen));
    out.write(name.c_str(), nameLen);

    out.write(reinterpret_cast<const char*> (&hp), sizeof(hp));
    out.write(reinterpret_cast<const char*> (&max_hp), sizeof(max_hp));
    int styleID = static_cast<int>(combatStyle);
    out.write(reinterpret_cast<const char*> (&styleID), sizeof(styleID));
    out.write(reinterpret_cast<const char*> (&posX), sizeof(posX));
    out.write(reinterpret_cast<const char*> (&posY), sizeof(posY));
    out.write(reinterpret_cast<const char*> (&isAlive), sizeof(isAlive));
}

void Character::load(std::istream& in) {
    in.read(reinterpret_cast<char*> (&id), sizeof(id));
    size_t nameLen;
    in.read(reinterpret_cast<char*> (&nameLen), sizeof(nameLen));
    name.resize(nameLen);
    in.read(&name[0], nameLen);

    in.read(reinterpret_cast<char*> (&hp), sizeof(hp));
    in.read(reinterpret_cast<char*> (&max_hp), sizeof(max_hp));
    
    int styleID;
    in.read(reinterpret_cast<char*> (&styleID), sizeof(styleID));
    combatStyle = static_cast<CombatStyle>(styleID);
    
    in.read(reinterpret_cast<char*> (&posX), sizeof(posX));
    in.read(reinterpret_cast<char*> (&posY), sizeof(posY));
    in.read(reinterpret_cast<char*> (&isAlive), sizeof(isAlive));
}