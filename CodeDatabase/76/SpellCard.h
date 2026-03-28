#pragma once
#include <string>
#include <memory>

using namespace std;

class Player;
class Enemy;

class SpellCard {
public:
    virtual ~SpellCard() = default; 
    
    virtual string GetName() const = 0;
    virtual string GetDescription() const = 0; 
    virtual int GetManaCost() const = 0; 
    virtual bool CanCast(const Player& caster, Enemy& enemy) const = 0;
    virtual void Cast(Player& caster, Enemy& enemy) = 0;
};