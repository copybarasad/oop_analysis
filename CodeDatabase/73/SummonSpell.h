#ifndef GAMELB1_SUMMONSPELL_H
#define GAMELB1_SUMMONSPELL_H

#include "ISpell.h"

class SummonSpell : public ISpell {
public:
    SummonSpell(int count, int health, int damage);

    void apply(Game& game, int target_x, int target_y) override;
    std::unique_ptr<ISpell> clone() const override;
    std::string getName() const override;
    std::string getDescription() const override;

    int getTypeId() const override;

    int& getCount();

private:
    int count_;
    int health_;
    int damage_;
};

#endif //GAMELB1_SUMMONSPELL_H