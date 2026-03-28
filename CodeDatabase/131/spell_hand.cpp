#include "spell_hand.h"
#include "player.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include "summon_spell.h"
#include "enhancement_spell.h"
#include <random>
#include <algorithm>

namespace rpg {
    SpellHand::SpellHand(size_t max_size)
        : max_size_(max_size), next_cast_enhanced_(false) {
        spells_.reserve(max_size);
    }

    SpellHand::SpellHand(const SpellHand &other)
        : max_size_(other.max_size_), next_cast_enhanced_(other.next_cast_enhanced_) {
        spells_.reserve(other.spells_.size());
        for (const auto &spell: other.spells_) {
            spells_.push_back(spell->Clone());
        }
    }

    SpellHand &SpellHand::operator=(const SpellHand &other) {
        if (this != &other) {
            max_size_ = other.max_size_;
            next_cast_enhanced_ = other.next_cast_enhanced_;
            spells_.clear();
            spells_.reserve(other.spells_.size());
            for (const auto &spell: other.spells_) {
                spells_.push_back(spell->Clone());
            }
        }
        return *this;
    }

    bool SpellHand::AddSpell(std::unique_ptr<Spell> spell) {
        if (IsFull()) {
            return false;
        }
        spells_.push_back(std::move(spell));
        return true;
    }

    [[nodiscard]] std::optional<size_t> SpellHand::BuyRandomSpell(Player &player) {
        if (IsFull()) {
            return std::nullopt;
        }

        auto spell = CreateRandomSpell();
        int cost = spell->GetCost();

        if (player.GetScore() < cost) {
            return std::nullopt;
        }

        player.AddScore(-cost);
        size_t index = spells_.size();
        spells_.push_back(std::move(spell));

        return index;
    }

    [[nodiscard]] const Spell *SpellHand::GetSpell(size_t index) const {
        if (index >= spells_.size()) {
            return nullptr;
        }
        return spells_[index].get();
    }

    [[nodiscard]] Spell *SpellHand::GetSpellMutable(size_t index) {
        if (index >= spells_.size()) {
            return nullptr;
        }
        return spells_[index].get();
    }

    void SpellHand::RemoveSpell(size_t index) {
        if (index < spells_.size()) {
            spells_.erase(spells_.begin() + index);
        }
    }

    [[nodiscard]] size_t SpellHand::GetSize() const {
        return spells_.size();
    }

    [[nodiscard]] size_t SpellHand::GetMaxSize() const {
        return max_size_;
    }

    [[nodiscard]] bool SpellHand::IsFull() const {
        return spells_.size() >= max_size_;
    }

    void SpellHand::InitializeWithRandomSpell() {
        spells_.clear();
        spells_.push_back(CreateRandomSpell());
    }

    [[nodiscard]] bool SpellHand::IsNextCastEnhanced() const {
        return next_cast_enhanced_;
    }

    void SpellHand::ActivateEnhancement() {
        next_cast_enhanced_ = true;
    }

    void SpellHand::DeactivateEnhancement() {
        next_cast_enhanced_ = false;
    }

    [[nodiscard]] std::unique_ptr<Spell> SpellHand::CreateRandomSpell() const {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dist(0, 4);

        int spell_type = dist(gen);

        switch (spell_type) {
            case 0:
                return std::make_unique<DirectDamageSpell>();
            case 1:
                return std::make_unique<AreaDamageSpell>();
            case 2:
                return std::make_unique<TrapSpell>();
            case 3:
                return std::make_unique<SummonSpell>();
            case 4:
                return std::make_unique<EnhancementSpell>();
            default:
                return std::make_unique<DirectDamageSpell>();
        }
    }
}
