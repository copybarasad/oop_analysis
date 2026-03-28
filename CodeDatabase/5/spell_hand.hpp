#ifndef SPELL_HAND_HPP
#define SPELL_HAND_HPP

#include "spell.hpp"
#include <memory>
#define MAX_SIZE 3

class Player;
class Enemy;
class GameField;
class Spell;

class Hand{
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int max_size;
public:
    Hand(int max_size=MAX_SIZE);
    void addSpell(std::unique_ptr<Spell> spell);
    void useSpell(int index, Player& player, int cordX,int cordY,std::vector<Enemy>& enemies,const GameField& field);
    int get_maxSize()const;
    int get_spellCount()const;
    bool isFull()const;
    void display_hand()const;//вывод содержимого руки
    const Spell* getSpell(int index) const;
    void removeRandomSpell();
};

#endif