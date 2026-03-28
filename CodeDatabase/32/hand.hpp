#ifndef HAND_HPP
#define HAND_HPP

#include <vector>
#include <memory>
#include <iostream>
#include "spell.hpp"

class Game;

class Hand {
private:
    std::vector<std::unique_ptr<Spell>> spells_;
    size_t capacity_;
    int enhancementStacks_ = 0;
    std::vector<int> upgrade_levels_;

public:
    explicit Hand(size_t capacity);

    bool AddSpell(std::unique_ptr<Spell> spell);
    bool UseSpell(size_t index, Game& game);
    void PrintHand() const;
    size_t Size() const noexcept { return spells_.size(); }

    void AddEnhancement(int amount = 1) { enhancementStacks_ += amount; }
    int GetEnhancementStacks() const { return enhancementStacks_; }
    void ResetEnhancements() { enhancementStacks_ = 0; }
    void RemoveRandomHalf();

    void UpgradeSpell(size_t index);

    void Clear();
    Spell* GetSpell(size_t index);
    const Spell* GetSpell(size_t index) const;
    int GetUpgradeLevel(size_t index) const;
    bool AddSpellWithLevel(std::unique_ptr<Spell> spell, int level);
};

#endif