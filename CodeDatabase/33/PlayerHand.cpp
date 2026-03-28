#include "PlayerHand.h"
#include "Spell.h"
#include <iostream>
#include <algorithm>
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BoostSpell.h"

PlayerHand::~PlayerHand() = default;

PlayerHand::PlayerHand(std::size_t max_size)
    : max_size_(max_size),
      spells_(),
      boost_direct_range_(0),
      boost_area_size_(0),
      boost_trap_damage_(0),
      boost_summon_count_(0) {}

PlayerHand::PlayerHand(const PlayerHand& other)
    : max_size_(other.max_size_)
    , boost_direct_range_(other.boost_direct_range_)
    , boost_area_size_(other.boost_area_size_)
    , boost_trap_damage_(other.boost_trap_damage_)
    , boost_summon_count_(other.boost_summon_count_)
{
    spells_.reserve(other.spells_.size());
    for (const auto& sp : other.spells_) {
        spells_.push_back(sp ? sp->Clone() : nullptr);
    }
}

PlayerHand& PlayerHand::operator=(const PlayerHand& other) {
    if (this == &other) return *this;
    max_size_ = other.max_size_;
    boost_direct_range_  = other.boost_direct_range_;
    boost_area_size_     = other.boost_area_size_;
    boost_trap_damage_   = other.boost_trap_damage_;
    boost_summon_count_  = other.boost_summon_count_;

    std::vector<std::unique_ptr<Spell>> fresh;
    fresh.reserve(other.spells_.size());
    for (const auto& sp : other.spells_) {
        fresh.push_back(sp ? sp->Clone() : nullptr);
    }
    spells_ = std::move(fresh);
    return *this;
}

std::size_t PlayerHand::Size() const { return spells_.size(); }
std::size_t PlayerHand::Capacity() const { return max_size_; }

void PlayerHand::PrintHand() const {
    std::cout << "Spells:\n";
    for (std::size_t i = 0; i < spells_.size(); ++i)
        std::cout << "  [" << i << "] " << spells_[i]->Name() << "\n";
    if (spells_.empty()) std::cout << "  (no spells)\n";
}

bool PlayerHand::AddSpell(std::unique_ptr<Spell> spell) {
    if (spells_.size() >= max_size_) return false;
    spells_.push_back(std::move(spell));
    return true;
}

void PlayerHand::AddRandomSpell(std::mt19937& rng) {
    if (spells_.size() >= max_size_) return;

    std::uniform_int_distribution<int> dist(0,4);
    int pick = dist(rng);

    std::unique_ptr<Spell> s;
    switch (pick) {
        case 0: s.reset(new DirectDamageSpell(5, 3)); break;
        case 1: s.reset(new AreaDamageSpell(4, 2)); break;
        case 2: s.reset(new TrapSpell(4)); break;
        case 3: s.reset(new SummonSpell(1, 10, 3)); break;
        default:s.reset(new BoostSpell(1)); break;
    }
    spells_.push_back(std::move(s));
}

bool PlayerHand::CastSpellByIndex(int index, Game& game) {
    if (index < 0 || static_cast<std::size_t>(index) >= spells_.size()) {
        std::cout << "Invalid spell index.\n";
        return false;
    }
    bool applied = spells_[index]->Cast(game, *this);
    if (applied) {
        spells_.erase(spells_.begin() + index);
        return true;
    }
    return false;
}

void PlayerHand::AddBoostAll(int v) {
    if (v <= 0) return;
    boost_direct_range_ += v;
    boost_area_size_ += v;
    boost_trap_damage_ += v;
    boost_summon_count_ += v;
    std::cout << "Boost stored for your next non-boost spell.\n";
}

void PlayerHand::ResetAllBoosts_() {
    boost_direct_range_ = 0;
    boost_area_size_ = 0;
    boost_trap_damage_ = 0;
    boost_summon_count_ = 0;
}

int PlayerHand::ConsumeBoostForDirectRange() {
    int b = boost_direct_range_;
    ResetAllBoosts_();
    return b;
}

int PlayerHand::ConsumeBoostForAreaSize() {
    int b = boost_area_size_;
    ResetAllBoosts_();
    return b;
}

int PlayerHand::ConsumeBoostForTrapDamage() {
    int b = boost_trap_damage_;
    ResetAllBoosts_();
    return b;
}

int PlayerHand::ConsumeBoostForSummonCount() {
    int b = boost_summon_count_;
    ResetAllBoosts_();
    return b;
}

void PlayerHand::IncreaseCapacity(size_t delta) { max_size_ += delta; }
void PlayerHand::ClearAllSpells() { spells_.clear(); }

void PlayerHand::RemoveHalfRandom(std::mt19937& rng) {
    if (spells_.empty()) return;
    std::vector<size_t> idx(spells_.size());
    for (size_t i=0;i<idx.size();++i) idx[i]=i;
    std::shuffle(idx.begin(), idx.end(), rng);
    size_t cut = idx.size()/2;
    std::sort(idx.begin(), idx.begin()+cut, std::greater<size_t>());
    for (size_t k=0;k<cut;++k) spells_.erase(spells_.begin()+idx[k]);
}

const std::vector<std::unique_ptr<Spell>>& PlayerHand::Spells() const { return spells_; }
