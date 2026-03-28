#include "SpellHolder.h"

#include <algorithm>
#include <random>

#include "SpellRegistry.h"
#include "world/World.h"

class Spell;

bool SpellHolder::add_spell(const std::string& spell) {
    if (!SpellRegistry::instance().get(spell)) return false;
    if (spells_.size() >= max_size_)
        return false;

    spells_.push_back(spell);
    return true;
}

std::vector<std::string> SpellHolder::get_spells() const {
    return spells_;
}

Spell *SpellHolder::get_selected() const {
    if (selected_spell_ < 0 || selected_spell_ >= spells_.size())
        return nullptr;

    return SpellRegistry::instance().get(spells_[selected_spell_]);
}

int SpellHolder::get_selected_index() const {
    return selected_spell_;
}

bool SpellHolder::remove_spell(const int index) {
    if (index < 0 || index >= static_cast<int>(spells_.size()))
        return false;

    spells_.erase(spells_.begin() + index);

    if (selected_spell_ > 0 && selected_spell_ >= index) {
        selected_spell_--;
    }

    return true;
}


bool SpellHolder::set_selected(const int index) {
    if (index < 0 || index >= static_cast<int>(spells_.size()))
        return false;

    selected_spell_ = index;
    return true;
}

void SpellHolder::unselect() {
    selected_spell_ = -1;
}

void SpellHolder::set_spell_buff(const int buff) {
    spell_buff_ = std::max(0, buff);
}

int SpellHolder::get_spell_buff() const {
    return spell_buff_;
}

bool SpellHolder::add_random_spell() {
    if (spells_.size() >= max_size_)
        return false;

    const auto& registry = SpellRegistry::instance();
    const auto spell_ids = registry.ids();

    if (spell_ids.empty())
        return false;

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, static_cast<int>(spell_ids.size()) - 1);
    const int index = dist(rng);

    const bool added = add_spell(spell_ids[index]);
    return added;
}


json SpellHolder::serialize() const {
    json data;

    json spells = json::array();
    for (const auto& spell : spells_) {
        spells.push_back(spell);
    }
    data["spells"] = spells;

    data["selected_spell"] = selected_spell_;
    data["spell_buff"] = spell_buff_;
    data["max_size"] = max_size_;

    return data;
}
bool SpellHolder::deserialize(json data) {
    try {
        clear();

        if (data.contains("max_size"))
            max_size_ = data["max_size"];
        if (data.contains("spell_buff"))
            spell_buff_ = data["spell_buff"];
        if (data.contains("selected_spell"))
            selected_spell_ = data["selected_spell"];

        if (data.contains("spells")) {
            for (auto& entry : data["spells"]) {
                const std::string& spell_id = entry.get<std::string>();
                if (SpellRegistry::instance().get(spell_id)) {
                    add_spell(spell_id);
                }
            }
        }
        if (spell_buff_ <= 0) spell_buff_ = 0;
        if (selected_spell_ < 0 || selected_spell_ >= spells_.size()) selected_spell_ = -1;

        return true;
    } catch (const std::exception& e) {
        std::cerr << "SpellHolder deserialization failed: " << e.what() << "\n";
        return false;
    }
}

void SpellHolder::remove_half_spells() {

    const int total = static_cast<int>(spells_.size());
    if (total == 0) return;

    const int to_remove = total / 2;

    std::vector<int> indices(total);
    std::iota(indices.begin(), indices.end(), 0);

    static std::mt19937 rng(std::random_device{}());
    std::ranges::shuffle(indices, rng);

    std::sort(indices.begin(), indices.begin() + to_remove, std::greater<>());

    for (int i = 0; i < to_remove; ++i) {
        remove_spell(indices[i]);
    }
}


