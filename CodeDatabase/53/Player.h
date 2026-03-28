#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "AttackType.h"
#include "Cell.h"
#include "ISpell.h"
#include <string>
#include <memory> 

class Enemy;
class Barracks;
class Saloon;
class GameField;
class HandSpell;
class CowboyTower;
class Teamate;

class Player : public Character {
private:
    int score;
    int coins;
    int level;
    int xp;
    AttackType currentAttackType;
    int meleeDamage;
    int rangedDamage;
    int attackRange;
    std::unique_ptr<HandSpell> handSpell;


public:

    Player(std::string playerName, int startX, int startY);
    
    
    void take_damage(int amount) override;
    bool move(char direction, GameField& field);
    void attack(char direction, GameField& field);
    bool checkAndAttack(int x, int y, GameField& field);
    void switchAttackType();

    void add_coins(int amount);
    void add_score(int amount);
    void add_xp(int amount);
    
    void damageBaracks(Barracks& barrack, int inpDamage);
    void damageEnemy(Enemy& enemy, int inpDamage);
    void damageCowboyTower(CowboyTower& tower, int inpDamage);
    void takeAwards(Type_cell type);

    bool useSpell(SpellID spellID, int x, int y, GameField& field);
    bool addSpell(SpellID spellID, int count);
    void removeRandomSpells(int count);
    
    // Геттеры
    int getScore() const;
    int getCoins() const;
    int getLevel() const;
    int getXp() const;
    int getMeleeDamage() const;
    int getRangedDamage() const;
    AttackType getAttackType() const;
    int getAttackRange() const;
    std::string getAttackTypeString() const;
    HandSpell* getHandSpell() const;
    // Сеттеры
    void setName(const std::string& newName);
    void setScore(int newScore);
    void setCoins(int newCoins);
    void setAlive(bool isAlive);
    void setLevel(int newLevel);
    void setXp(int newXp);
    void setAttackType(AttackType type);
    void setMeleeDamage(int damage);
    void setRangedDamage(int damage);
    void setAttackRange(int range);

    void upgradeMeleeDamage(int amount);
    void upgradeRangedDamage(int amount);
    
};

#endif