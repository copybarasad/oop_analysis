#ifndef ENHANCEMENTSPELL_H
#define ENHANCEMENTSPELL_H

#include "ISpell.hpp"

class EnhancementSpell : public ISpell {
private:
    int damage_;
    int range_;
    int manaCost_;
    SpellType type_;

    int damageBonus_ = 0;
    int rangeBonus_ = 0;
    int areaBonus_ = 0;
    int summonBonus_ = 0;

public:
    EnhancementSpell(int damageBonus = 5, int rangeBonus = 1, int areaBonus = 1, int summonBonus = 1, int manaCost = 20);
    
    bool cast(int targetX, int targetY, GameState& gameState, SpellCaster& caster) override;
    std::unique_ptr<ISpell> clone() const override;
    void applyEnhancements(int enhancementCount) override;

    SpellType getType() const override { return type_;}

    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
    
    int getManaCost() const { return manaCost_; }
    int getDamageBonus() const { return damageBonus_; }
    int getRangeBonus() const { return rangeBonus_; }
    int getAreaBonus() const { return areaBonus_; }
    int getSummonBonus() const { return summonBonus_; }
    
    void setManaCost(int manaCost) { manaCost_ = manaCost; }
    void setDamageBonus(int bonus) { damageBonus_ = bonus; }
    void setRangeBonus(int bonus) { rangeBonus_ = bonus; }
    void setAreaBonus(int bonus) { areaBonus_ = bonus; }
    void setSummonBonus(int bonus) { summonBonus_ = bonus; }
};

#endif