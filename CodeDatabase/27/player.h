#pragma once 
#include "unit.h"
#include "point.h"
#include <memory>
#include <vector>
#include "hand.h"

class Hand;

class Player : public Unit {
protected:
    void on_move_to_point(Point& point) override;

private:
    enum class disk {melee, range};
    disk current_disk;
    int score;
    std::unique_ptr<Hand> hand;
    int max_hp;

    static constexpr int MELEE_DAMAGE = 25;
    static constexpr int RANGE_DAMAGE = 15;

public:
    ~Player();
    Player(size_t hand_capacity);
    void switch_weapon();
    int get_score() const { return score; }
    void add_score(int points) { score += points; }
    Hand* get_hand() const { return hand.get(); }
    void increase_damage(int delta);
    void increase_max_hp(int amt);
    void heal(int amount);
    void set_hp(int v);
    int get_max_hp() const { return max_hp; }
    void set_max_hp(int v) { max_hp = v; if (hp > max_hp) hp = max_hp; }
    int get_disk_type() const { return static_cast<int>(current_disk); }
    void set_disk_type(int v);
};


