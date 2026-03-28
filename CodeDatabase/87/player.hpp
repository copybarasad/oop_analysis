#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "character.hpp"
#include "player_hand.hpp"

enum class Combat_style{
    CLOSE_COMBAT,
    RANGED_COMBAT
};

class Player: public Character{
    private:
        int score;
        Combat_style combat_style;
        int close_damage;
        int ranged_damage;
        Player_hand hand;
    public:
        Player(int health, int close_damage_value, int ranged_damage_value, Position start_position, int hand_size = 2);
        void switch_combat_style(Combat_style new_style);
        Combat_style get_combat_style() const;
        int get_score() const;
        void add_score(int points);
        void take_damage(int damage_amount) override;
        Player_hand& get_hand(){return hand;}
        const Player_hand& get_hand() const {return hand;}
        void gain_spell();
        bool buy_spell(int cost = 20);
        
        int get_close_damage() const{return close_damage;}
        int get_ranged_damage() const{return ranged_damage;}

        void set_hand(Player_hand&& new_hand);
        void update_alive_state(){check_alive_state();}

        void upgrade_damage(int amount);
        void upgrade_spell_damage(int amount);
};

#endif