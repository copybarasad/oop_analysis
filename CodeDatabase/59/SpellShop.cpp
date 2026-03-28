#include "SpellShop.h"

#include <algorithm>
#include <format>
#include <random>
#include <string>
#include <vector>
#include "Constants.h"

SpellShop::SpellShop(int count, std::pair<int, int> price_range,
    std::pair<int, int> damage_range, float aoe_damage_multiplier) :
    price_range(price_range), damage_range(damage_range),
    aoe_damage_multiplier(aoe_damage_multiplier) {
    count = std::max(0, std::min(count, 9));
    for (int i = 0; i < count; i++) {
        spells_with_prices.push_back(createRandomSpellWithPrice());
    }
}

std::pair<std::unique_ptr<BasicSpell>, int> SpellShop::createRandomSpellWithPrice() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::vector<std::string> direct_names = {"Огненный шар", "Ледяная стрела", "Удар молнии"};
    std::vector<std::string> aoe_names = {"Ледяная буря", "Огненный шторм", "Землетрясение"};
    std::vector<std::string> trap_names = {"Медвежий капкан", "Ядовитая ловушка", "Магическая сеть"};

    std::uniform_int_distribution<> type_dist(0, 2);
    std::uniform_int_distribution<> damage_dist(damage_range.first, damage_range.second);
    std::uniform_int_distribution<> range_dist(2, 5);
    std::uniform_int_distribution<> name_dist(0, 2);
    std::uniform_int_distribution<> price_dist(price_range.first, price_range.second);

    BasicSpell::Type type = static_cast<BasicSpell::Type>(type_dist(gen));
    int base_damage = damage_dist(gen);
    int range = range_dist(gen);
    int price = price_dist(gen);

    std::string name;
    int final_damage = base_damage;
    switch (type) {
        case BasicSpell::Type::Direct:
            name = direct_names[name_dist(gen)];
            break;
        case BasicSpell::Type::AoE:
            name = aoe_names[name_dist(gen)];
            final_damage = static_cast<int>(base_damage * aoe_damage_multiplier);
            break;
        case BasicSpell::Type::Trap:
            name = trap_names[name_dist(gen)];
            range = 2;
            break;
    }

    std::unique_ptr<BasicSpell> spell;
    switch (type) {
        case BasicSpell::Type::Direct:
        case BasicSpell::Type::AoE:
            spell = std::make_unique<DamagingSpell>(type, final_damage, range, name);
            break;
        case BasicSpell::Type::Trap:
            spell = std::make_unique<TrapSpell>(type, final_damage, range, name);
            break;
        default:
            spell = std::make_unique<DamagingSpell>(BasicSpell::Type::Direct, final_damage, range, name);
    }
    return {std::move(spell), price};
}

std::unique_ptr<BasicSpell> SpellShop::buySpell(int index) {
    if (index < 0 || index >= spells_with_prices.size()) {
        return nullptr;
    }
    auto& spell_pair = spells_with_prices[index];
    auto spell = std::move(spell_pair.first);
    spells_with_prices.erase(spells_with_prices.begin() + index);
    spells_with_prices.push_back(createRandomSpellWithPrice());
    return spell;
}

std::string SpellShop::toString() {
    std::string str = "Доступные заклинания:\n";
    for (int i = 0; i < spells_with_prices.size(); i++) {
        str += std::format("{}. {}\tЦена: {}\n", i+1, spells_with_prices[i].first->toString(), spells_with_prices[i].second);
    }
    str += "0. Отмена покупки";
    return str;
}

const BasicSpell* SpellShop::getSpell(int index) const {
    if (index < 0 || index >= spells_with_prices.size()) { return nullptr;
    }
    return spells_with_prices[index].first.get();
}

void SpellShop::addSpell(std::unique_ptr<BasicSpell> spell, int price) {
    spells_with_prices.emplace_back(std::move(spell), price);
}

json SpellShop::toJson() const {
    json j = json::array();
    for(const auto& pair : spells_with_prices) {
        j.push_back({
            {"spell", pair.first->toJson()},
            {"price", pair.second}
        });
    }
    return j;
}

void SpellShop::fromJson(const json& j) {
    spells_with_prices.clear();
    for (const auto& item_json : j) {
        const auto& spell_json = item_json.at("spell");
        int price = item_json.at("price").get<int>();

        BasicSpell::Type type = spell_json.at("type").get<BasicSpell::Type>();

        std::unique_ptr<BasicSpell> newSpell;
        if (type == BasicSpell::Type::Trap) {
            newSpell = std::make_unique<TrapSpell>(type, 0, 0, "");
        } else {
            newSpell = std::make_unique<DamagingSpell>(type, 0, 0, "");
        }

        newSpell->fromJson(spell_json);

        spells_with_prices.emplace_back(std::move(newSpell), price);
    }
}

void SpellShop::regenerate() {
    spells_with_prices.clear();
    int count = std::max(0, std::min(SHOP_SPELLS_COUNT, 9));
    for (int i = 0; i < count; i++) {
        spells_with_prices.push_back(createRandomSpellWithPrice());
    }
}