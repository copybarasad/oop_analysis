#ifndef PLAYER_H
#define PLAYER_H

#include "Creature.h" 

class Game;

class Player: public Creature{
    bool is_melee = true;
    int kills = 0;
    int melee_damage = PLAYER_MELEE_DAMAGE;
    int range_damage = PLAYER_RANGE_DAMAGE;
    int melee_range = PLAYER_MELEE_RANGE;
    int far_range = PLAYER_FAR_RANGE;
    int current_range = this->melee_range;
public:
    
    Player();
    int getCurrentHP() const { return current_hp; }
    int getKills() const { return kills; }
    int getCurrentDamage() const { return current_damage; }
    int getCurrentRange() const { return current_range; }
    bool getIsMelee() const { return is_melee; }
    int getRange() const; 
    void change_fight_style();
    void kill_up();
    int kills_to_spells();
    void save(std::ofstream& ofs) const override;
    void load(std::ifstream& ifs) override;
    friend class Game;
};

#endif