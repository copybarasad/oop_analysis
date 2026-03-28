#pragma once
#include <memory>
#include <utility>
#include <vector>
#include "BasicSpell.h"
#include "DamagingSpell.h"
#include "TrapSpell.h"

class SpellShop {
private:
    std::pair<int, int> price_range;
    std::pair<int, int> damage_range;
    float aoe_damage_multiplier;
    std::vector<std::pair<std::unique_ptr<BasicSpell>, int>> spells_with_prices;
public:
    SpellShop(int count, std::pair<int, int> price_range, std::pair<int, int> damage_range, float aoe_damage_multiplier);
    SpellShop(const SpellShop&) = delete;
    SpellShop& operator=(const SpellShop&) = delete;
    SpellShop(SpellShop&&) = default;
    SpellShop& operator=(SpellShop&&) = default;
    ~SpellShop() = default;
    std::unique_ptr<BasicSpell> buySpell(int index);
    int getSpellPrice(int index) { return spells_with_prices[index].second; }
    std::string toString();
    int getCount() { return spells_with_prices.size();
}
    std::unique_ptr<BasicSpell> createRandomSpell() { return createRandomSpellWithPrice().first; }

    const BasicSpell* getSpell(int index) const;
    void addSpell(std::unique_ptr<BasicSpell> spell, int price);
    void clear() { spells_with_prices.clear(); }

    void regenerate();

    json toJson() const;
    void fromJson(const json& j);

private:
    std::pair<std::unique_ptr<BasicSpell>, int> createRandomSpellWithPrice();
};