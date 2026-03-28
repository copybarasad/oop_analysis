#ifndef GAMELB1_TRAPSPELL_H
#define GAMELB1_TRAPSPELL_H

#include "ISpell.h"

class TrapSpell : public ISpell {
public:
    explicit TrapSpell(int damage);

    void apply(Game& game, int target_x, int target_y) override;
    std::unique_ptr<ISpell> clone() const override;
    std::string getName() const override;
    std::string getDescription() const override;

    int getTypeId() const override;

    int& getDamage();

private:
    int damage_;
};

#endif //GAMELB1_TRAPSPELL_H