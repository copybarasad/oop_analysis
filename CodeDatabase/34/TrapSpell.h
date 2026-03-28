#ifndef GAME_TRAP_SPELL_H
#define GAME_TRAP_SPELL_H

#include "ISpell.h"

namespace Game {
class TrapSpell : public ISpell {
public:
    TrapSpell(int baseDamage = 30);
    bool cast(Game& game, const Utils::Point& target, int enhancementStacks) override;
    std::string getName() const override;
    std::string getDescription() const override;
    bool requiresTarget() const override;

    SpellType getType() const override;
    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
private:
    int baseDamage;
};
}
#endif