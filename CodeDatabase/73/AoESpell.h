#ifndef GAMELB1_AOESPELL_H
#define GAMELB1_AOESPELL_H

#include "ISpell.h"

class AoESpell : public ISpell {
public:
    AoESpell(int damage, int cast_radius, int area_radius);

    void apply(Game& game, int target_x, int target_y) override;
    std::unique_ptr<ISpell> clone() const override;
    std::string getName() const override;
    std::string getDescription() const override;

    int getTypeId() const override;

private:
    int damage_;
    int cast_radius_;
    int area_radius_;
};

#endif //GAMELB1_AOESPELL_H