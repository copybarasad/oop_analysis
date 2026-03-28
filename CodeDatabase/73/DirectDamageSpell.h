#ifndef GAMELB1_DIRECTDAMAGESPELL_H
#define GAMELB1_DIRECTDAMAGESPELL_H

#include "ISpell.h"

class DirectDamageSpell : public ISpell {
public:
    DirectDamageSpell(int damage, int radius);

    void apply(Game& game, int target_x, int target_y) override;
    std::unique_ptr<ISpell> clone() const override;
    std::string getName() const override;
    std::string getDescription() const override;

    int getTypeId() const override;

    int& getRadius();

private:
    int damage_;
    int radius_;
};

#endif //GAMELB1_DIRECTDAMAGESPELL_H