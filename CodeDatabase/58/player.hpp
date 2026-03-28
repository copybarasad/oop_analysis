#pragma once

#include <functional>
#include <algorithm>

#include "entity.hpp"
#include "enum_type_attack.hpp"
#include "player_hand.hpp"

#define BASE_POINTS 0
#define MIN_POINTS 0
#define MAX_POINTS 1000
#define BASE_HP_PLAYER 20
#define BASE_DAMAGE_PLAYER 5
#define BASE_TYPE_ATTACK TypeAttack::MELEE

class Player : public Entity{
private:
    int points{0};
    bool is_slowed{false};
    TypeAttack type_attack{BASE_TYPE_ATTACK};
    PlayerHand hand;

    int validate_points(int p);
public:
    // using Entity::Entity;
    Player(int hp = BASE_HP_PLAYER, int damage = BASE_DAMAGE_PLAYER,
        TypeAttack type_attack = BASE_TYPE_ATTACK, int size_hand = BASE_SIZE_HAND);
    virtual ~Player() override = default;

    int get_points();
    void add_points(int p = 1);
    void set_points(int p);

    void switch_slowed();
    bool has_slowed();

    void switch_attack();
    TypeAttack get_type_attack();
    void set_type_attack(TypeAttack type_attack);

    PlayerHand& get_hand();
    void print_avaible_cards();

    void update_player();

    virtual int get_damage() override;
    virtual char get_symbol() override;
};