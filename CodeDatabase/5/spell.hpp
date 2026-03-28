#ifndef SPELL_HPP
#define SPELL_HPP

#include "player.hpp"
#include "enemy.hpp"
#include "game_field.hpp"
#include <string>
#include <iostream>

class Player;
class Enemy;
class GameField;

class Spell
{
protected: 
    std::string name;
    std::string disc;
public:
    Spell(const std::string& name,const std::string& disc);
    virtual void cast_spell(Player& player, int cordX,int cordY,std::vector<Enemy>& enemies,const GameField& field)=0;
    std::string get_name()const;
    std::string get_disc()const;
    virtual std::string getType() const = 0;
    virtual ~Spell()=default;
};

#endif