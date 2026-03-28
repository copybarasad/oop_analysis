#ifndef SPELL_INTERFACE_H
#define SPELL_INTERFACE_H

#include <string>
#include <memory>

class GameMap;

class ISpell
{
public:
    virtual ~ISpell() = default;
    
    virtual int get_mana_cost() const = 0;
    virtual std::string get_name() const = 0;
    virtual int get_cooldown() const = 0;
    virtual int get_max_cooldown() const = 0;
    virtual int get_cast_range() const = 0;

    virtual void set_cooldown(int value) = 0;
    
    virtual bool is_available(int player_mana) const = 0;
    virtual void update_cooldown() = 0;
    
    virtual void cast(GameMap& map, int target_x, int target_y) = 0;

    virtual void save_binary(std::ostream& os) const = 0;
    virtual void load_binary(std::istream& is) = 0;
};

#endif