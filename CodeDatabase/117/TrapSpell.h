#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "ISpell.h"
#include "../Characters/Trap.h"
#include "../Constants.h"

class TrapSpell: public ISpell{
private:
    int damage;
    int cost;
    std::pair<int, int> position;

public:
    TrapSpell(std::pair<int, int> position);
    bool cast(GameState& gameState);
    void setDMG(int damage);
    int getDMG() const;
    void upgrade(int power) override;
    std::string getName() const override;
    int getCost() const override;
};

#endif