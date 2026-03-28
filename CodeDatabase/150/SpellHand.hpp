#ifndef SPELLHAND_H
#define SPELLHAND_H

#include "ISpell.hpp"
#include "DirectDamageSpell.hpp"
#include "AreaDamageSpell.hpp"
#include "TrapSpell.hpp"
#include "SummonSpell.hpp"
#include "EnhancementSpell.hpp"
#include <vector>
#include <memory>
#include <random>

class SpellHand {
private:
    std::vector<std::unique_ptr<ISpell>> spells_;
    int maxSize_;
    mutable std::mt19937 randomGenerator_;

public:
    explicit SpellHand(int maxSize = 5);
    SpellHand(const SpellHand& other);
    SpellHand(SpellHand&& other) noexcept;
    SpellHand& operator=(const SpellHand& other);
    SpellHand& operator=(SpellHand&& other) noexcept;
    void swap(SpellHand& other);

    ~SpellHand() = default;
    
    bool addSpell(std::unique_ptr<ISpell> spell);
    bool removeSpell(int index);
    void clear();
    
    ISpell* getSpell(int index) const;
    int getSpellCount() const;
    int getMaxSize() const;
    
    void fillWithRandomSpells(int count);
    std::unique_ptr<ISpell> takeSpell(int index);

    void serialize(std::ostream& os) const;
    void deserialize(std::istream& is);
    
private:
    std::unique_ptr<ISpell> createRandomSpell() const;
};

#endif