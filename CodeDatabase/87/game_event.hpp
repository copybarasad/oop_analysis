#ifndef GAME_EVENT_HPP
#define GAME_EVENT_HPP
#include "position.hpp"
#include <string>
#include <memory>

class Game_event{
    public:
        virtual ~Game_event() = default;
        virtual std::string to_string() const = 0;
};

class Player_move_event: public Game_event{
    public:
        Position from, to;
        Player_move_event(Position f, Position t): from(f), to(t){}
        std::string to_string() const override{
            return "player_move=" + std::to_string(from.get_x()) + "," + std::to_string(from.get_y()) + 
                "-->" + std::to_string(to.get_x()) + "," + std::to_string(to.get_y());
        }
};

class Enemy_move_event: public Game_event{
    public:
        std::string enemy_id;
        Position from, to;
        Enemy_move_event(std::string id, Position f, Position t): enemy_id(std::move(id)), from(f), to(t){}
        std::string to_string() const override{
            return "enemy_move=" + enemy_id + ":(" + std::to_string(from.get_x()) + "," + std::to_string(from.get_y()) +
                ")-->(" + std::to_string(to.get_x()) + "," + std::to_string(to.get_y()) + ")";
        }
};

class Spell_cast_event: public Game_event{
    public:
        std::string spell_name;
        std::string target;
        int damage;
        Spell_cast_event(std::string name, std::string tgt, int dmg): spell_name(std::move(name)), target(std::move(tgt)), damage(dmg){}
        std::string to_string() const override{return "spell_cast=" + spell_name + "-->" + target + ":" + std::to_string(damage);}
};

class Damage_event: public Game_event{
    public:
        std::string source, target;
        int damage;
        Damage_event(std::string src, std::string tgt, int dmg): source(std::move(src)), target(std::move(tgt)), damage(dmg){}
        std::string to_string() const override{return "damage=" + source + "-->" + target + ":" + std::to_string(damage);}
};

class Spell_add_event: public Game_event{
    public:
        std::string spell_name;
        Spell_add_event(std::string name): spell_name(std::move(name)){}
        std::string to_string() const override{return "spell_add=" + spell_name;}
};

class Level_complete_event: public Game_event{
    public:
        int level;
        Level_complete_event(int lvl): level(lvl){}
        std::string to_string() const override{return "level_completed=" + std::to_string(level - 1);}
};

class Game_over_event: public Game_event{
    public:
        bool player_won;
        Game_over_event(bool won): player_won(won){}
        std::string to_string() const override{return "game_over=" + std::string(player_won ? "win" : "lose");}
};

#endif