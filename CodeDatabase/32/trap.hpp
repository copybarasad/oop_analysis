#ifndef TRAP_HPP
#define TRAP_HPP

#include "spell.hpp"
#include <string>

class Game;

class TrapSpell : public Spell {
private:
    int damage_;
    int radius_;
public:
    TrapSpell(int damage = 20, int radius = 1);
    std::string GetName() const noexcept override;
    bool UseSpell(Game& game) override;
    void Upgrade(); 
};

#endif