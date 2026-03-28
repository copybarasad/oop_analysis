#ifndef HAND_H
#define HAND_H

#include "Spell.h"
#include "SpellTypes.h"
#include <vector>
#include <memory>
#include <random>

class Hand {
private:
    std::vector<std::unique_ptr<Spell>> spells_;
    size_t maxSize_;
    EnhancementState enhancementState_;
    EnhancementState permanentBonuses_;
    std::mt19937 rng_;

public:
    explicit Hand(size_t maxSize = 5);

    bool addSpell(std::unique_ptr<Spell> spell);

    size_t getSpellCount() const;

    size_t getMaxSize() const;

    const Spell* getSpell(size_t index) const;

    bool castSpell(size_t index, SpellContext& context);

    const EnhancementState& getEnhancementState() const;

    static std::unique_ptr<Spell> getRandomSpell(std::mt19937& rng);
    static std::unique_ptr<Spell> createSpell(SpellType type, const std::string& name);
    
    // Methods for managing spells
    void removeSpellAt(size_t index);
    void clearAllSpells();
    void addPermanentSpellBonus(const EnhancementState& bonus);
};

#endif

