#ifndef SPELL_H
#define SPELL_H

#include <string>

class game_field;
class Entity;

class Spell {
protected:
    std::string name;
    int damage;
    int radius;
    
public:
    Spell(const std::string& spell_name, int spell_damage, int spell_radius);
    virtual ~Spell() = default;
    virtual bool canCast(game_field* field, int player_x, int player_y, int target_x, int target_y) const = 0;
    virtual void apply(game_field* field, int player_x, int player_y, int target_x, int target_y, Entity* target = nullptr) = 0;
    std::string getName() const;
    int getDamage() const;
    int getRadius() const;
    void buffDamage(int amount);
};

#endif