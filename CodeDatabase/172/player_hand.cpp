#include "player_hand.h"
#include "ispell.h"
#include "spell_factory.h"

#include "error.h"

#include <iostream>

namespace game {
    PlayerHand::PlayerHand(const PlayerHand& other)
        : cap_(other.cap_)
    {
        cards_.reserve(other.cards_.size());
        for (const auto& ptr: other.cards_) {
            cards_.push_back(ptr ? ptr->clone() : nullptr);
        }
    }

    PlayerHand& PlayerHand::operator=(const PlayerHand& other)
    {
        if (this == &other) return *this;
        
        cap_ = other.cap_;

        std::vector<std::unique_ptr<ISpell>> tmp;
        tmp.reserve(other.cards_.size());
        for (const auto& ptr: other.cards_) {
            tmp.push_back(ptr ? ptr->clone() : nullptr);
        }

        cards_.swap(tmp);

        return *this;
    }

    // ----------- Give random spell to hand --------------
    bool PlayerHand::addRandomSpell(const GameConfig& cfg, std::mt19937& rng) 
    {
        if (full()) return false;

        auto spell = SpellFactory::createRandom(rng, cfg);
        return add(std::move(spell));
    }

    bool PlayerHand::removeRandomHalf(std::mt19937& rng)
    {
        if (size() < 2) return false;

        size_t half = size() / 2;

        for (size_t i = 0; i < half; ++i) {
            std::uniform_int_distribution<size_t> index(0, size() - 1);
            removeAt(index(rng));
        }

        return true;
    }

    bool PlayerHand::upgradeRandomSpell(const GameConfig& cfg, std::mt19937& rng,
                                        const ISpell** upgradedSpell)
    {
        if (upgradedSpell) *upgradedSpell = nullptr;
        if (empty()) return false;

        std::uniform_int_distribution<size_t> dist(0, size() - 1);
        size_t index = dist(rng);

        auto& randomSpell = cards_[index];
        if (!randomSpell) return false;

        randomSpell->applyLevelUp(cfg);

        if (upgradedSpell) {
            *upgradedSpell = randomSpell.get();
        }
        
        return true;
    }

    void PlayerHand::save(std::ostream& out) const
    {
        const size_t sz = size();
        out << sz << '\n';

        for (size_t i = 0; i < sz; ++i) {
            const ISpell* spell = at(i);

            if (!spell) {
                out << -1 << '\n';
                continue;
            }

            const SpellId id = spell->id();
            out << static_cast<int>(id) << ' '
                << spell->getDamage()   << ' '
                << spell->getRadius()   << '\n';
        }
    }

    void PlayerHand::load(std::istream& in, const GameConfig& cfg)
    {
        cards_.clear();

        size_t sz = 0;
        if (!(in >> sz)) {
            throw LoadError("Failed to read hand size from save file");
        }

        if (sz > (size_t)(cap_)) {
            sz = cap_;
        }

        for (size_t i = 0; i < sz; ++i) {
            int rawId = -1;
            int dmg   = 0;
            int r     = 0;
            if (!(in >> rawId >> dmg >> r)) {
                throw LoadError("Failed to read spell from save file");
            }

            if (rawId < 0) {
                cards_.push_back(nullptr);
                continue;
            }

            SpellId id = static_cast<SpellId>(rawId);
            auto spell = SpellFactory::createById(id, cfg);
            
            spell->setDamage(dmg);
            spell->setRadius(r);

            cards_.push_back(std::move(spell));
        }
    }
}