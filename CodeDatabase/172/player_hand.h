#pragma once

#include <vector>
#include <memory>
#include <random>
#include <optional>

#include "ispell.h"
#include "gameconfig.h"

namespace game {
    class PlayerHand {
        private:
            size_t cap_;
            std::vector<std::unique_ptr<ISpell>> cards_;
        public:
            explicit PlayerHand(size_t capacity) : cap_(capacity)
            {}

            PlayerHand(const PlayerHand& other);
            PlayerHand& operator=(const PlayerHand& other);

            PlayerHand(PlayerHand&& other) noexcept = default;
            PlayerHand& operator=(PlayerHand&& other) noexcept = default;

            size_t size() const     { return cards_.size(); }
            size_t capacity() const { return cap_; }
            bool   full() const     { return size() >= cap_; }
            bool   empty() const    { return cards_.empty(); }
            
            bool addRandomSpell(const GameConfig& cfg, std::mt19937& rng);
            bool removeRandomHalf(std::mt19937& rng);
            bool upgradeRandomSpell(const GameConfig& cfg, std::mt19937& rng, const ISpell** upgradedSpell);

            bool add(std::unique_ptr<ISpell> card) {
                if (full() || !card) return false;

                cards_.push_back(std::move(card));
                return true;
            }

            ISpell* atMutable(size_t index) {
                if (index >= cards_.size()) return nullptr;

                return cards_[index].get();
            }

            const ISpell* at(size_t index) const {
                if (index >= cards_.size()) return nullptr;

                return cards_[index].get();
            }

            bool removeAt(size_t index) {
                if (index >= cards_.size()) return false;

                cards_.erase(cards_.begin() + static_cast<std::ptrdiff_t>(index));
                return true;
            }

            void save(std::ostream& out) const;
            void load(std::istream& in, const GameConfig& cfg);
    };
}