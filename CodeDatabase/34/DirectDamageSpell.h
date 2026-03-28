#ifndef GAME_DIRECT_DAMAGE_SPELL_H
#define GAME_DIRECT_DAMAGE_SPELL_H

#include "ISpell.h"

namespace Game {

class DirectDamageSpell : public ISpell {
public:
    DirectDamageSpell(int baseDamage = 25, int baseRange = 5);
    
    bool cast(Game& game, const Utils::Point& target, int enhancementStacks) override;
    bool cast(Game& game, const Utils::Point& casterPosition, const Utils::Point& target);

    std::string getName() const override;
    std::string getDescription() const override;
    bool requiresTarget() const override;
    
    SpellType getType() const override;
    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;

private:
    int baseDamage;
    int baseRange;
};

}

#endif