#pragma once

#include "Position.h"
#include "Combat_mode.h"

class Player {
private:
    int health;
    int max_Health;
    int melee_Damage;
    int ranged_Damage;
    int score;
    CombatMode current_Mode;
    bool isSlowed;
    Position position;

public:
    Player(int start_x, int start_y, int health = 100, int melee_Damage = 20, int ranged_Damage = 15);

    void take_Damage(int damage);
    void Hill(int hill_points);
    void add_Score(int points);
    void switch_CombatMode();
    void apply_Slow();
    void remove_Slow();

    bool is_Alive() const;
    int get_Health() const;
    int get_Score() const;
    CombatMode get_CombatMode() const;
    int get_Damage() const;
    bool get_IsSlowed() const;
    const Position& get_Position() const;

    void set_Position(const Position& newPosition);
    
    int get_MaxHealth() const { return max_Health; }
    int get_MeleeDamage() const { return melee_Damage; }
    int get_RangedDamage() const { return ranged_Damage; }
    
    void setMaxHealth(int health);
    void setMeleeDamage(int damage);
    void setRangedDamage(int damage);
    void increaseMaxHealth(int amount);
    void increaseMeleeDamage(int amount);
    void increaseRangedDamage(int amount);
    
    void setHealth(int h);
    void setScore(int s);
    void setCombatMode(CombatMode mode);
    void setSlowState(bool slow);
};
