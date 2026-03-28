#pragma once
#include <iostream>
#include <random>
#include <vector>

#include <nlohmann/json.hpp>

#include "core/render/renderer/IRenderer.h"


class World;
class SpellRegistry;
class Spell;

using json = nlohmann::json;

class SpellHolder {
public:
    explicit SpellHolder(size_t max_size) : max_size_(max_size) {
    }

    bool add_spell(const std::string& spell);

    bool remove_spell(int index);

    std::vector<std::string> get_spells() const;

    Spell *get_selected() const;

    int get_selected_index() const;

    bool set_selected(int index);

    void unselect();

    void set_spell_buff(int buff);

    int get_spell_buff() const;

    bool add_random_spell();

    void clear() {
        spells_.clear();
        selected_spell_ = -1;
    }

    json serialize() const;
    bool deserialize(json data);

    void remove_half_spells();

private:
    size_t max_size_;
    std::vector<std::string> spells_;
    int selected_spell_ = 0;
    int spell_buff_ = 0;
};
