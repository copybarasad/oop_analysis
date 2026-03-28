#include "Player.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>

Player::Player(bool IsPlayer, bool IsAlive, ATKtype type, std::string Body, int HP, int ATK, int X_coord, int Y_coord, int Speed, int Points)
    :Humanoid(true, true, ATKtype::melee, "ꙮ ", HP, ATK, X_coord, Y_coord, Speed), 
     Points(Points), KDA(0), Souls(10), Mana(3), stance(type), maxSpells(5), 
     upgradeBuffer(0) 
{
    resetSpeedCD(0);
    resetATKCD(0);
    addSpell(static_cast<int>(SpellType::DEATH_NOTE), 1);
    updateAttackStats();
}

void Player::updateAttackStats() 
{
    if (stance == ATKtype::melee) 
    {
        attackRange = 1;
        attackDamage = 3;
    } 
    else 
    {
        attackRange = 3;
        attackDamage = 1;
        
        for (const auto& spell : storeSpells) 
        {
            if (spell.first == static_cast<int>(SpellType::DEATH_NOTE)) 
            {
                attackRange = 2 + spell.second;
                break;
            }
        }
    }
}

void Player::reset() 
{
    IsPlayer = true;
    IsAlive = true;
    type = ATKtype::melee;
    Body = "ꙮ ";
    HP = 9;
    maxHP = 9;
    ATK = 3;
    Speed = 1;
    XY_coord.setXY_coord(1, 1);
    where = Direction::KNOWHERE;
    
    Points = 0;
    KDA = 0;
    Souls = 10;
    Mana = 3;
    storeSpells.clear();
    stance = ATKtype::melee;
    maxSpells = 5;
    upgradeBuffer = 0;
    
    resetSpeedCD(0);
    resetATKCD(0);
    addSpell(static_cast<int>(SpellType::DEATH_NOTE), 1);
    updateAttackStats();
}

void Player::addKDA (int n) { KDA += n; }
void Player::addSoul (int n) { Souls += n; }
void Player::spendSoul (int n) { Souls -= n; if (Souls < 0) Souls = 0; }

void Player::addSpell(int spellEnum, int LvL) 
{
    if (storeSpells.size() < maxSpells) storeSpells.push_back(std::make_pair(spellEnum, LvL));
    else std::cout << "Spell hand is full! Max spells: " << maxSpells << "\n";
    updateAttackStats();
}

void Player::spendSpell(int spellEnum) 
{
    for(int i = 0; i < storeSpells.size(); i++) 
    {
        if(storeSpells[i].first == spellEnum) 
        {
            storeSpells.erase(storeSpells.begin() + i);
            break;
        }
    }
    updateAttackStats();
}

void Player::upgradeSpell(int spellEnum) 
{
    for(int i = 0; i < storeSpells.size(); i++) 
    {
        if(storeSpells[i].first == spellEnum) 
        {
            storeSpells[i].second++;
            break;
        }
    }
    updateAttackStats();
}

std::vector<std::pair<int, int>> Player::listSpells() const { return storeSpells; }
int Player::getSouls() const { return Souls; }

void Player::switchAttackType() 
{
    if (stance == ATKtype::melee) 
    {
        stance = ATKtype::ranged;
    } 
    else 
    {
        stance = ATKtype::melee;
    }
    updateAttackStats();
}

ATKtype Player::getAttackType() const { return stance; }
int Player::getAttackRange() const { return attackRange; }
int Player::getAttackDamage() const { return attackDamage; }

std::vector<Coordinates> Player::getAttackCoordinates() const 
{
    std::vector<Coordinates> targets;
    for (int dx = -attackRange; dx <= attackRange; dx++) 
    {
        for (int dy = -attackRange; dy <= attackRange; dy++) 
        {
            if (dx == 0 && dy == 0) continue;
            if (abs(dx) + abs(dy) <= attackRange) 
            {
                targets.push_back(Coordinates(XY_coord.getX_coord() + dx, 
                                            XY_coord.getY_coord() + dy));
            }
        }
    }
    return targets;
}

Coordinates Player::moveto(Direction where) 
{
    switch(where) 
    {
        case Direction::KNOWHERE: return Coordinates(this->getX_coord(),this->getY_coord());
        case Direction::UP: return Coordinates(this->getX_coord(),this->getY_coord()-1);
        case Direction::DOWN: return Coordinates(this->getX_coord(),this->getY_coord()+1);
        case Direction::LEFT: return Coordinates(this->getX_coord()-1,this->getY_coord());
        case Direction::RIGHT: return Coordinates(this->getX_coord()+1,this->getY_coord());
        default: return Coordinates(this->getX_coord(),this->getY_coord());
    }
}

bool Player::move(Coordinates destination, int damage, int Speed, int cash) 
{
    this->setXY_coord(destination.getX_coord(), destination.getY_coord());
    this->take_damage(damage);
    if (this->checkPulse() == false) return false;
    this->addPoints(cash);
    this->setSpeed(Speed);
    return true;
}

int Player::checkPoints() const { return Points; }
void Player::addPoints(int cash) { this->Points = this->Points+cash; if (this->checkPoints()<0) this->Points = 0; }
void Player::setMaxSpells(int newMax) { maxSpells = newMax; }
int Player::getMaxSpells() const { return maxSpells; }

void Player::removeRandomSpells(int count) 
{
    if (count >= storeSpells.size()) 
    {
        storeSpells.clear();
        addSpell(static_cast<int>(SpellType::DEATH_NOTE), 1);
    } 
    else 
    {
        std::vector<int> indicesToRemove;
        while (indicesToRemove.size() < count && storeSpells.size() > 1) 
        {
            int index = rand() % storeSpells.size();
            if (storeSpells[index].first != static_cast<int>(SpellType::DEATH_NOTE)) 
            {
                bool alreadySelected = false;
                for (int idx : indicesToRemove) 
                {
                    if (idx == index) 
                    {
                        alreadySelected = true;
                        break;
                    }
                }
                if (!alreadySelected) 
                {
                    indicesToRemove.push_back(index);
                }
            }
        }
        
        std::sort(indicesToRemove.rbegin(), indicesToRemove.rend());
        for (int index : indicesToRemove) 
        {
            storeSpells.erase(storeSpells.begin() + index);
        }
    }
    updateAttackStats();
}

void Player::addUpgradeBuffer() 
{ 
    upgradeBuffer++; 
    std::cout << "Upgrade stored! Total: " << upgradeBuffer << "\n";
}

void Player::applyUpgradeBuffer(SpellType spellType) 
{
    if (upgradeBuffer > 0) 
    {
        std::cout << "Applying " << upgradeBuffer << " upgrades to spell...\n";
        for (int i = 0; i < upgradeBuffer; i++) 
        {
            upgradeSpell(static_cast<int>(spellType));
        }
        upgradeBuffer = 0;
    }
}

int Player::getUpgradeBuffer() const { return upgradeBuffer; }

std::string Player::serialize() const 
{
    std::stringstream ss;
    ss << Humanoid::serialize() << "\n";
    ss << Points << " " << KDA << " " << Souls << " " << Mana << " "
       << static_cast<int>(stance) << " " << maxSpells << " "
       << attackRange << " " << attackDamage << " " << upgradeBuffer << "\n";
    ss << storeSpells.size() << "\n";
    for (const auto& spell : storeSpells) 
    {
        ss << spell.first << " " << spell.second << "\n";
    }
    return ss.str();
}

void Player::deserialize(const std::string& data) 
{
    std::stringstream ss(data);
    std::string humanoidData;
    std::getline(ss, humanoidData);
    Humanoid::deserialize(humanoidData);
    
    int stanceInt;
    ss >> Points >> KDA >> Souls >> Mana >> stanceInt >> maxSpells 
    >> attackRange >> attackDamage >> upgradeBuffer;
    stance = static_cast<ATKtype>(stanceInt);
    
    int spellCount;
    ss >> spellCount;
    storeSpells.clear();
    for (int i = 0; i < spellCount; i++) 
    {
        int spellType, spellLevel;
        ss >> spellType >> spellLevel;
        storeSpells.push_back(std::make_pair(spellType, spellLevel));
    }
}