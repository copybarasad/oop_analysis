#pragma once
#include <vector>
#include <memory>
#include <random>
#include "ISpell.h"

class Player;
class GameField;

class Hand {
    std::vector<std::unique_ptr<ISpell>> cards_;
    std::size_t cap_;
public:
    explicit Hand(std::size_t cap) : cap_{cap} {}

    std::size_t Size() const noexcept { return cards_.size(); }
    std::size_t Capacity() const noexcept { return cap_; }

    bool Add(std::unique_ptr<ISpell> s) {
        if (!s || cards_.size() >= cap_) return false;
        cards_.push_back(std::move(s));
        return true;
    }

    bool Use(std::size_t idx, Player& p, GameField& f, const SpellContext& ctx, bool removeIfUsed) {
        if (idx >= cards_.size()) return false;
        bool spent = cards_[idx]->Cast(p, f, ctx);
        if (removeIfUsed && spent)
            cards_.erase(cards_.begin() + static_cast<long>(idx));
        return spent;
    }

    const ISpell* Get(std::size_t idx) const noexcept {
        return (idx < cards_.size()) ? cards_[idx].get() : nullptr;
    }
    void Clear() {
    cards_.clear();
    }
        // Удалить случайным образом половину карт
    void TrimHalfRandom() {
        if (cards_.size() <= 1) return;

        std::size_t toRemove = cards_.size() / 2;
        std::mt19937 rng(std::random_device{}());

        for (std::size_t i = 0; i < toRemove && !cards_.empty(); ++i) {
            std::uniform_int_distribution<std::size_t> dist(0, cards_.size() - 1);
            std::size_t idx = dist(rng);
            cards_.erase(cards_.begin() + static_cast<long>(idx));
        }
    }


};