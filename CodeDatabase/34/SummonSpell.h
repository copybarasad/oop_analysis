#ifndef GAME_SUMMON_SPELL_H
#define GAME_SUMMON_SPELL_H

#include "ISpell.h"

namespace Game {
class SummonSpell : public ISpell {
public:
    SummonSpell(int baseSummons = 1);
    bool cast(Game& game, const Utils::Point& target, int enhancementStacks) override;
    std::string getName() const override;
    std::string getDescription() const override;
    bool requiresTarget() const override;

    SpellType getType() const override;
    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
private:
    int baseSummons;
};
}
#endif