#pragma once
#include <vector>
#include <memory>
#include <random>
#include "Spell.h"
#include "BuffSpell.h"

class Field;
class Player;

class Hand {
private:
    std::vector<std::shared_ptr<Spell>> spells_;
    size_t maxSize_;
    std::vector<BuffSpell::BuffType> pendingBuffs_;

public:
    explicit Hand(size_t maxSize = 5);
    bool addSpell(std::shared_ptr<Spell> spell);
    std::vector<std::shared_ptr<Spell>> getAvailableSpells() const;
    bool useSpell(size_t index, Field& field, Player& player, int tx, int ty);
    void addBuff(BuffSpell::BuffType type);
    void applyBuffsTo(Spell& spell);
    void clearBuffs();
    bool removeRandomSpell();
    bool removeSpell(size_t index);
    void addRandomSpell();
    void increaseMaxSize(int amount) { maxSize_ += amount; }
    size_t size() const { return spells_.size(); }
    size_t getMaxSize() const { return maxSize_; }
};