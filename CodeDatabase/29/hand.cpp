#include "hand.hpp"
#include "spell.hpp"
#include "game.hpp"
#include <algorithm>
#include <numeric>
#include <functional>

PlayerHand::PlayerHand(std::size_t cap):capacity_(cap){}
PlayerHand::~PlayerHand() = default;

std::size_t PlayerHand::Capacity() const noexcept { return capacity_; }
std::size_t PlayerHand::Size() const noexcept { return spells_.size(); }
bool PlayerHand::Add(std::unique_ptr<ISpell> s){
    if (!s) return false;
    if (spells_.size()>=capacity_) return false;
    spells_.push_back(std::move(s));
    return true;
}
bool PlayerHand::Use(std::size_t index, Game& game, int tx,int ty){
    if (index>=spells_.size()) return false;
    if (!spells_[index]) return false;
    if (spells_[index]->Use(game,tx,ty)){
        spells_.erase(spells_.begin()+static_cast<long>(index));
        return true;
    }
    return false;
}
const ISpell* PlayerHand::Get(std::size_t index) const{
    if (index>=spells_.size()) return nullptr;
    return spells_[index].get();
}
std::vector<std::string> PlayerHand::Names() const{
    std::vector<std::string> r;
    r.reserve(spells_.size());
    for (auto& p:spells_) r.emplace_back(p? p->Name():"");
    return r;
}

std::vector<std::string> PlayerHand::Serialize() const{
    std::vector<std::string> codes;
    codes.reserve(spells_.size());
    for (const auto& spell : spells_){
        codes.emplace_back(spell ? spell->Serialize() : std::string());
    }
    return codes;
}

void PlayerHand::Load(const std::vector<std::string>& codes){
    spells_.clear();
    for (const auto& code : codes){
        if (spells_.size() >= capacity_) break;
        if (code.empty()) continue;
        auto spell = ISpell::Deserialize(code);
        if (spell){
            spells_.push_back(std::move(spell));
        }
    }
}

void PlayerHand::DiscardHalf(std::mt19937& rng){
    if (spells_.size() <= 1) return;
    std::vector<std::size_t> indices(spells_.size());
    std::iota(indices.begin(), indices.end(), std::size_t(0));
    std::shuffle(indices.begin(), indices.end(), rng);
    std::size_t to_remove = spells_.size()/2;
    std::vector<std::size_t> victim(indices.begin(), indices.begin()+static_cast<long>(to_remove));
    std::sort(victim.begin(), victim.end(), std::greater<std::size_t>());
    for (std::size_t idx : victim){
        spells_.erase(spells_.begin()+static_cast<long>(idx));
    }
}
