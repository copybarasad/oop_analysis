#ifndef RPG_SPELLSHAND_H
#define RPG_SPELLSHAND_H

#include <vector>
#include <memory>
#include <random>
#include "ISpell.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class SpellsHand {
    std::vector<std::shared_ptr<ISpell>> spells;
    unsigned int maximumSize;
    int selectedSpell;
public:
    explicit SpellsHand(unsigned int spellsHandSize);

    explicit SpellsHand(json& data);

    virtual ~SpellsHand() = default;

    SpellsHand(const SpellsHand& other);

    SpellsHand& operator=(const SpellsHand& other);

    SpellsHand(SpellsHand&&) noexcept = default;

    SpellsHand& operator=(SpellsHand&&) noexcept = default;

    [[nodiscard]] json to_json() const;

    void addSpell(std::shared_ptr<ISpell> spell);

    [[nodiscard]] std::vector<std::shared_ptr<ISpell>> getSpells() const;

    [[nodiscard]] bool empty() const;

    void reload(bool full=false) const;

    [[nodiscard]] ISpell* getFirstReadySpell() const;

    [[nodiscard]] std::vector<ISpell*> getReadySpells() const;

    ISpell* selectSpell(int spellIndex);

    void clearSelection();

    ISpell* getSelectedSpell();

    [[nodiscard]] unsigned int getMaxSize() const;

    void addRandomSpell(std::mt19937 &gen);

    void processNextLevel(const std::vector<int>& protectedSpells, std::mt19937 gen);
};

#endif
