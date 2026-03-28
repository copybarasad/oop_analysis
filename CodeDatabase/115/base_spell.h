#ifndef BASE_SPELL_H
#define BASE_SPELL_H

#include "spell.h"
#include <string>

class BaseSpell : public Spell {
protected:
    std::string name;
    std::string description;
    int range;
    int cost;
    bool needs_target; 
    
public:
    BaseSpell(const std::string& spell_name, const std::string& desc, int spell_range, int spell_cost, bool needs_target);
    
    const std::string& get_name() const override { return name; }
    const std::string& get_description() const override { return description; }
    int get_range() const override { return range; }
    int get_cost() const override { return cost; }
    bool requires_target() const override { return needs_target; }  
    
    virtual void apply_enhancement(int enhancement_level) override = 0;
    virtual std::unique_ptr<Spell> clone() const override = 0;
    
    virtual ~BaseSpell() = default;
};

#endif