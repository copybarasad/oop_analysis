#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "ISpell.hpp"

class AreaDamageSpell : public ISpell {
private:
    int damage_;
    int range_;
    int manaCost_;
    SpellType type_;
    int areaSize_;

public:
    AreaDamageSpell(int damage = 15, int range = 2, int areaSize = 2, int manaCost = 15);
    
    bool cast(int targetX, int targetY, GameState& gameState, SpellCaster& caster) override;
    std::unique_ptr<ISpell> clone() const override;
    bool checkRange(int casterX, int casterY, int targetX, int targetY) const;
    void enhanceArea(int bonus);
    void applyEnhancements(int enhancementCount) override;
    
    SpellType getType() const override { return type_;}

    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
    
    int getDamage() const { return damage_; }
    int getRange() const { return range_; }
    int getManaCost() const { return manaCost_; }
    int getAreaSize() const { return areaSize_; }
    
    void setDamage(int damage) { damage_ = damage; }
    void setRange(int range) { range_ = range; }
    void setManaCost(int manaCost) { manaCost_ = manaCost; }
    void setAreaSize(int areaSize) { areaSize_ = areaSize; }
};

#endif