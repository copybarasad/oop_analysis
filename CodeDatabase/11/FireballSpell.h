#ifndef FIREBALLSPELL_H
#define FIREBALLSPELL_H

#include "ISpell.h"
class FireballSpell : public ISpell {
public:
    SpellType getType() const override { return SpellType::FIREBALL; }
    FireballSpell(int damage = 50, int range = 7); 

    bool cast(Player& caster, GameController& game) override;
    bool castByTower(int startX, int startY, int targetX, int targetY, GameController& game);

    std::string getName() const override;
    std::string getDescription() const override;

private:
    int damage;
    int range;
};

#endif