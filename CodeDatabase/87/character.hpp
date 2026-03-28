#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include "position.hpp"

class Character{
    protected:
        int health;
        int max_health;
        int damage;
        Position position;
        bool is_alive_flag;
        void check_alive_state();
    public:
        Character(int character_health, int character_damage, Position start_position);
        virtual ~Character() = default;

        virtual void move(const Position& new_position);
        virtual void take_damage(int damage_amount);
        virtual void heal(int heal_amount);

        bool is_alive() const;
        int get_health() const;
        int get_max_health() const;
        int get_damage() const;
        Position get_position() const;

        void set_position(const Position& new_position);
};

#endif