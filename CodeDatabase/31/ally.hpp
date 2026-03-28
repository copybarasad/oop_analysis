#ifndef ALLY_HPP
#define ALLY_HPP

#include "ai_entity.hpp"

enum class AllyAttackType { Melee, Ranged };

class Ally : public AIEntity {
    int melee_damage = 3;
    int ranged_damage = 1;
    int melee_radius = 1;
    int ranged_radius = 2;
    AllyAttackType attack_type = AllyAttackType::Melee;
    
public:
    Ally(Field &field, Point position = {0, 0}, int health = 12);
    Team get_team() const;
    EntityType get_entity_type() const;
    std::string make_step();
    bool attack(Point point);
    void switch_attack_type();
    std::string str() {return "ally";}
};

#endif