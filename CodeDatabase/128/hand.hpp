#pragma once
#include <memory>     
#include "spell.hpp"         
#include "enhancementrules.hpp"

class Hand {
public:
    explicit Hand(size_t capacity);

    bool AddSpell(std::unique_ptr<Spell> spell);
    bool UseSpell(size_t index, Game& game);
    void PrintHand() const;
    void GenerateRandomSpell();

    bool isFull() const noexcept { return spells_.size() >= capacity_; }
    size_t Size() const noexcept { return spells_.size(); }
    int getEnhancementStacks() const noexcept { return enhancement_stacks_; }
    const std::vector<std::unique_ptr<Spell>>& getSpells() const { return spells_; }

    void AddEnhancement(int value) noexcept { enhancement_stacks_ += value; }
    void ResetEnhancement() noexcept { enhancement_stacks_ = 0; }
    void SetEnhancementStacks(int value) { enhancement_stacks_ = value; }
    void RemoveHalfCards();
    size_t SpellCount() const { return spells_.size(); }
    void Clear() { spells_.clear(); }

private:
    const size_t capacity_;
    std::vector<std::unique_ptr<Spell>> spells_; 
    int enhancement_stacks_ = 0;
    EnhancementRules rules_;  
};