#pragma once

#include <map>
#include <vector>
#include "player.h"
#include "enemy.h"
#include "spell_hand.h"
class ISpell;

class PlayerManager {
private:
    std::pair<int, int> playerPosition;
    Player* player;
    SpellHand* spellHand;

public:
    PlayerManager(Player* newPlayer=nullptr, std::pair<int, int> pos={0,0});
    ~PlayerManager();

    void setPlayer(Player* newPlayer, int x, int y); 
    Player* getPlayer() const;
    int getPlayerHealth() const;
    bool getPlayerFightType() const;
    int getPlayerDamage() const;
    int getPlayerPoints() const;
    int getPlayerMaxHealth() const;
    void upgradePlayerRandomSpell();
    
    void addPlayerSpell(ISpell * spell);
    void awardPoints(int points);
    void healPlayer(int amount);
    void setPlayerMaxHealth(int health);
    void setPlayerDamage(int damage);
    void switchPlayerFightType();
    void takePlayerDamage(int damage);
    std::pair<int, int> getPlayerPosition() const;
    bool hasPlayerAt(int x, int y);
    void movePlayer(int x, int y);
    SpellHand& getSpellHandRef();
    SpellHand* getSpellHand() const;  
};