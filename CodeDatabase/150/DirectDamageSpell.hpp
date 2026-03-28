#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "ISpell.hpp"
#include <memory>

class DirectDamageSpell : public ISpell {
private:
    int damage_;
    int range_;
    int manaCost_;
    SpellType type_;

public:
    DirectDamageSpell(int damage = 20, int range = 3, int manaCost = 10);
    
    bool cast(int targetX, int targetY, GameState& gameState, SpellCaster& caster) override;
    std::unique_ptr<ISpell> clone() const override;
    bool checkRange(int casterX, int casterY, int targetX, int targetY) const;
    void enhanceRange(int bonus);
    void applyEnhancements(int enhancementCount) override;

    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
    
    SpellType getType() const override { return type_; }
    int getDamage() const { return damage_; }
    int getRange() const { return range_; }
    int getManaCost() const { return manaCost_; }
    
    void setDamage(int damage) { damage_ = damage; }
    void setRange(int range) { range_ = range; }
    void setManaCost(int manaCost) { manaCost_ = manaCost; }
};

#endif