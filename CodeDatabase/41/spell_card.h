#ifndef SPELL_CARD_H
#define SPELL_CARD_H

#include "game_field.h"
#include <string>

class Player;
class Game_field;

class Spell_card{
protected:
    std::string name;
    std::string description;
    int mana_cost;
    int range;
    bool enhanced;

public:
    Spell_card(const std::string& spell_name, const std::string& desc, int cost,  int range);
    virtual ~Spell_card() = default;

    std::string get_name() const;
    std::string get_description() const;
    int get_mana_cost() const;
    int get_range() const;
    bool is_enhanced() const { return enhanced; }
    virtual bool can_cast(const Player& player, int target_x, int target_y, const Game_field& field) const = 0;
    virtual void cast(Player& player, int target_x, int target_y, Game_field& field) = 0;
    virtual Spell_card* clone() const = 0;

    virtual void enhance() = 0;
    virtual Target_type get_target_type() const = 0;
};

#endif