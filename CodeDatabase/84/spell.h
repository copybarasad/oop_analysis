#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <memory>

enum class SpellType {
    DirectDamage,
    AoEDamage,
    Trap,
    Lightning
};

class Field;
class Player;

class Spell {
protected:
    std::string name_;
    std::string description_;
    int range_;
    int mana_cost_;

public:
    Spell(const std::string& name, const std::string& description, int range, int mana_cost);
    virtual ~Spell() = default;
    
    virtual std::string get_name() const;
    virtual std::string get_description() const;
    virtual bool can_cast(const Field& field, int target_x, int target_y) const;
    virtual bool cast(Field& field, int target_x, int target_y) = 0;
    virtual int get_range() const;
    virtual int get_mana_cost() const;
    virtual std::string get_short_info() const { return ""; }
    virtual SpellType get_type() const = 0;

protected:
    bool is_target_in_range(const Field& field, int target_x, int target_y) const;
    void apply_damage_to_target(Field& field, int target_x, int target_y, int damage) const;
};

#endif