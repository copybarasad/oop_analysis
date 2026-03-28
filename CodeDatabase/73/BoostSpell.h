#ifndef GAMELB1_BOOSTSPELL_H
#define GAMELB1_BOOSTSPELL_H

#include "ISpell.h"

class BoostSpell : public ISpell {
public:
    BoostSpell();

    void apply(Game& game, int target_x, int target_y) override;
    std::unique_ptr<ISpell> clone() const override;
    std::string getName() const override;
    std::string getDescription() const override;

    int getTypeId() const override;
};

#endif //GAMELB1_BOOSTSPELL_H
