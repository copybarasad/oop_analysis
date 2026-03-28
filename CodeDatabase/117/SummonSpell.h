#ifndef SUMMONSPELL_H
#define SUMMONSPELL_H

#include "ISpell.h"
#include "../Controllers/MovementSystem.h"
#include "../Constants.h"

class SummonSpell: public ISpell{
private:
    int alliesNum;
    MovementSystem movSys;
    int cost;
public:
    SummonSpell();
    void setAlliesNum(int num);
    int getAlliesNum() const;
    bool cast(GameState& gameState) override;
    void upgrade(int power) override;
    std::string getName() const override;
    int getCost() const override;
};

#endif