#pragma once
#include "Position.h"


struct GameConfig {
    int lvl_index;
    int sea_width;
    int sea_height;
    int health;
    int common_damage;
    int ranged_damage;
    int ranged_range;
    int pl_level;
    int pl_score;
    Position pl_start_pos;
    int en_count;
    int en_hp;
    int en_damage;
    int oct_count;
    int oct_hp;
    int oct_damage;
    int oct_range;
    int vortex_count;
    int boost_damage;
    int boost_level;
    int boost_score;
    int spells_count;
    int jet_spell_damage;
    int jet_spell_range;
    int jet_spell_cost;
    int mine_spell_damage;
    int mine_spell_range;
    int mine_spell_cost;
    int fish_net_damage;
    int fish_net_cost;
    int fish_net_range;
    
    GameConfig(int l_i = 1, int w = 10, int h = 10, 
               int hp = 10, int d = 9, int rd = 9,
               int l = 1, int s = 0,
               int ec = 1, int ehp = 1,
               int ed = 1, int oc = 2,
               int ohp = 1, int od = 3,
               int ocr = 2, int vc = 3,
               int bd = 2, int bl = 1,
               int bs = 10, int rr = 5,
               int sc = 3, int jsd = 8,
               int jsr = 6, int jsc = 30,
               int msd = 9, int msr = 4,
               int msc = 50, int fnd = 9,
               int fnc = 10, int fnr = 5);

    void saveTo(SaveFileRAII& out) const ;
    void loadFrom(SaveFileRAII& in);
};