#ifndef HAND_H 
#define HAND_H

#include "spell_stats.h"
#include "direct_damage.h"
#include "area_damage.h"
#include "trap.h"
#include "call_ally.h"
#include "upgrade.h"
#include "randomiser.h"
#include "json.hpp"

using json = nlohmann::json;


class Hand{
    protected:
    unsigned int count;
    unsigned int max_count;
    unsigned int upgrade;
    std::vector<Spell*> hand;

    public:
    Hand(unsigned int game_lvl);

    ~Hand();

    void get_spell(unsigned int game_lvl);

    void delete_spells();

    bool can_use_spell();

    Spell_stats use_spell(unsigned int index);

    json to_json() const;
    
    void from_json(const json& j, unsigned int game_lvl);
};

#endif