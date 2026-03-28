#include "PlayerHand.h"
#include <algorithm>
#include <random>

PlayerHand::PlayerHand(int max_size)
    : max_size_(max_size),
      enhancement_stacks_(0) {}

PlayerHand::~PlayerHand() = default;

bool PlayerHand::AddSpell(Spell* spell) {
    if (static_cast<int>(spells_.size()) >= max_size_) {
        delete spell;
        return false;
    }
    spells_.push_back(std::unique_ptr<Spell>(spell));
    return true;
}

bool PlayerHand::RemoveSpell(int index) {
    if (index < 0 || index >= static_cast<int>(spells_.size())) {
        return false;
    }
    spells_.erase(spells_.begin() + index);
    return true;
}

Spell* PlayerHand::GetSpell(int index) const {
    if (index < 0 || index >= static_cast<int>(spells_.size())) {
        return nullptr;
    }
    return spells_[index].get();
}

int PlayerHand::GetSpellCount() const {
    return static_cast<int>(spells_.size());
}

int PlayerHand::GetMaxSize() const {
    return max_size_;
}

void PlayerHand::AddEnhancementStack() {
    enhancement_stacks_++;
}

int PlayerHand::GetEnhancementStacks() const {
    return enhancement_stacks_;
}

void PlayerHand::ResetEnhancementStacks() {
    enhancement_stacks_ = 0;
}

void PlayerHand::RemoveRandomSpells(int count) {
    if (count <= 0 || spells_.empty()) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    int to_remove = std::min(count, static_cast<int>(spells_.size()));
    for (int i = 0; i < to_remove; ++i) {
        if (!spells_.empty()) {
            std::uniform_int_distribution<> dist(0, spells_.size() - 1);
            int index = dist(gen);
            spells_.erase(spells_.begin() + index);
        }
    }
}

void PlayerHand::Clear() {
    spells_.clear();
}
