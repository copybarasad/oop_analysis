#ifndef MAGIC_HPP
#define MAGIC_HPP

#include <vector>
#include <iostream>
#include "../map/gamemap.hpp"
#include "movement.hpp"
#include "combat.hpp"
#include "../entities/player.hpp"
#include "../entities/ally.hpp"
#include <sstream>
#include <algorithm>
#include <memory>

class MagicHand{
    int capacity;
    std::vector<char> inventory;
    GameMap& map;
    Movement movement;
    Combat combat;
    int spellRange;
    Player* player; 
    int spellDamage = 20;
    std::vector<std::unique_ptr<Character>>* allCharacters = nullptr;
    int improvementStacks = 0;

public:
    MagicHand(GameMap& map, int capacity=5);
    void setAllCharacters(std::vector<std::unique_ptr<Character>>* ptr) { allCharacters = ptr; }
    void cast(char spell);
    void getRandSpell();
    void getArgs(int& dx, int& dy, int& dist, bool fg);
    
    void handleInput(const std::string& command);
    void displayHand() const;
    void punish();
    int getSpellDamage(){return spellDamage;}
    void setSpellDamage(int nwDamage){spellDamage = nwDamage;}
    
    void getDirection(std::string direction, int& dx, int& dy);
    const std::vector<char>& getInventory() const { return inventory; }
    void setInventory(json& newInventory);
    void setPlayer(Player* t_player) {player = t_player;}
    Player* getPlayer(){return player;}

    void thunderbolt(int dx, int dy, int dist, Character* mage=nullptr, Player* player1=nullptr);
    void brimstone(int dx, int dy);
    void sunstrike(int dx, int dy, int dist);
    void snare(int dx, int dy, int dist);
    void summon();
    void improve();

};

#endif