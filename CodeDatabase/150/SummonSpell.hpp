#ifndef SUMMONSPELL_H
#define SUMMONSPELL_H

#include "ISpell.hpp"

class SummonSpell : public ISpell {
private:
    int range_;
    int manaCost_;
    SpellType type_;
    int summonCount_;

public:
    SummonSpell(int summonCount = 1, int range = 1, int manaCost = 20);
    
    bool cast(int targetX, int targetY, GameState& gameState, SpellCaster& caster) override;
    std::unique_ptr<ISpell> clone() const override;
    bool checkRange(int casterX, int casterY, int targetX, int targetY) const;
    void enhanceSummon(int bonus);
    void applyEnhancements(int enhancementCount) override;

    SpellType getType() const override { return type_;}

    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;
    
    int getRange() const { return range_; }
    int getManaCost() const { return manaCost_; }
    int getSummonCount() const { return summonCount_; }
    
    void setRange(int range) { range_ = range; }
    void setManaCost(int manaCost) { manaCost_ = manaCost; }
    void setSummonCount(int count) { summonCount_ = count; }
};

#endif