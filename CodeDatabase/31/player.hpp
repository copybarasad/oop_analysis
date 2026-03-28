#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "entity.hpp"
#include "human.hpp"
#include <nlohmann/json_fwd.hpp>

class Hand;

enum class AttackType { 
    Melee = 0, 
    Ranged = 1,
    TotalCount = 2
};

enum class Action {exit, left, right, up, down, attack, switch_attack_type, skip_step, show_hand, buy_spell, use_spell};

class Player : public Human {
    int melee_damage = 3;
    int ranged_damage = 1;
    int melee_radius = 1;
    int ranged_radius = 2;
    AttackType attack_type  {AttackType::Melee};
    bool is_skip_step = {false};
    int count_skips = 0;
    int score = 15;
    Action action;
    Point action_point {0, 0};
    
public:
    Player(Field &field, Point position = {0, 0}, int health = 99);
    
    
    void add_score(int points); 
    int get_score() const; 
    void decrease_score(int value);
    void switch_attack_type();
    AttackType get_attack_type() const;
    Entity& attack(Point point);
    void set_action(Action action);
    void set_point(Point point);
    void set_position(Point pos);
    std::string make_step();
    void upgrade_damage();
    int get_melee_damage() const;
    int get_ranged_damage() const;
    int get_ranged_radius() const;
    nlohmann::json save() const;
    Team get_team() const;
    EntityType get_entity_type() const;
    Point rand_enemy();
    std::string str() {return "player";}
    int get_current_damage() {
        return (attack_type == AttackType::Melee) ? melee_damage : ranged_damage;
    }
};

#endif
