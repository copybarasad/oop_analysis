#pragma once

#include "Humanoid.h"
#include <vector>
#include <utility>
#include <string>
#include <sstream>

enum class SpellType 
{
    DEATH_NOTE,
    PSI_STORM, 
    TRAP,
    DOPPELGANGER,
    LEVEL_UP
};

class Player:public Humanoid 
{
protected:
    int Points;
    int KDA;
    int Souls;
    int Mana;
    std::vector<std::pair<int, int>> storeSpells;
    ATKtype stance;
    int maxSpells;
    int attackRange;
    int attackDamage;
    int upgradeBuffer;
public:
    Player(bool IsPlayer, bool IsAlive, ATKtype type, std::string Body, int HP, int ATK, int X_coord, int Y_coord, int Speed, int Points = 0);
    
    void updateAttackStats();
    void reset();
    void addKDA (int n);
    void addSoul (int n);
    void spendSoul (int n);
    void addSpell(int spellEnum, int LvL);
    void spendSpell(int spellEnum);
    void upgradeSpell(int spellEnum);
    std::vector<std::pair<int, int>> listSpells() const;
    int getSouls() const;
    void switchAttackType();
    ATKtype getAttackType() const;
    int getAttackRange() const;
    int getAttackDamage() const;
    std::vector<Coordinates> getAttackCoordinates() const;
    Coordinates moveto(Direction where);
    bool move(Coordinates destination, int damage, int Speed, int cash);
    int checkPoints() const;
    void addPoints(int cash);
    void setMaxSpells(int newMax);
    int getMaxSpells() const;
    void removeRandomSpells(int count);
    void addUpgradeBuffer();
    void applyUpgradeBuffer(SpellType spellType);
    int getUpgradeBuffer() const;
    
    std::string serialize() const;
    void deserialize(const std::string& data);
};