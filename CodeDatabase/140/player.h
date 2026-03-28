#pragma once
#include "hand.h"
#include <vector>

enum class Mode {
    CLOSE,
    FAR
};

class Player {
private:
    int health;
    int close_damage;
    int far_damage;
    int score;
    Mode current_mode;
    bool mode_switch_cooldown;
    bool is_slowed;
    bool slowed_this_turn;
    Hand hand;
    int enemies_killed;
    int level_up = 0;

    void add_random_spell();

public:
    Player(int init_health, int close_dmg, int far_dmg, int hand_size=5);
    Player(const Player&);
    Player& operator=(const Player&);
    Player(Player&&) noexcept;
    Player& operator=(Player&&) noexcept;

    void take_damage(int damage);
    bool is_alive() const;
    int get_health() const;

    Mode get_mode() const;
    void switch_mode();
    bool has_mode_cooldown() const;
    void clear_mode_cooldown();
    int get_current_damage() const;

    void apply_slowdown();
    void slowdown_turn();
    bool is_currently_slowed() const;

    int get_score() const;
    void add_score(int points);

    void increment_killed();

    const Hand& get_hand() const;
    Hand& get_hand();

    void add_level_up();
    int get_level() const;
    void consume_levels();

    void upgrade_max_health();
    void upgrade_close_damage();
    void upgrade_far_damage();
};