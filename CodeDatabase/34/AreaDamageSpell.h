#ifndef GAME_AREA_DAMAGE_SPELL_H
#define GAME_AREA_DAMAGE_SPELL_H

#include "ISpell.h"

namespace Game {

class AreaDamageSpell : public ISpell {
public:
    AreaDamageSpell(int damage = 100, int baseAreaSize = 2);

    bool cast(Game& game, const Utils::Point& target, int enhancementStacks) override;
    std::string getName() const override;
    std::string getDescription() const override;
    bool requiresTarget() const override;
    
    SpellType getType() const override;
    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;

private:
    int damage;
    int baseAreaSize;
};
}
#endif