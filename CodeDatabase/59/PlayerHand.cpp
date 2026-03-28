#include "PlayerHand.h"
#include <format>
#include <string>

#include "DamagingSpell.h"
#include "TrapSpell.h"

bool PlayerHand::removeSpell(int index) {
    if (index < 0 || index >= spells.size()) {
        return false;
    }
    spells.erase(spells.begin() + index);
    return true;
}

std::string PlayerHand::toString() {
    if (spells.empty()) {
        return "Инвентарь заклинаний пуст";
    }

    std::string result = "Заклинания в инвентаре:\n";
    for (int i = 0; i < spells.size(); ++i) {
        result += std::format("{}. {}\n", i + 1, spells[i]->toString());
    }
    result += "0. Отмена";
    return result;
}

BasicSpell* PlayerHand::getSpell(int index) {
    if (index < 0 || index >= spells.size()) {
        return nullptr;
    }
    return spells[index].get();
}

json PlayerHand::toJson() const {
    json j = json::array();
    for(const auto& spell : spells) {
        j.push_back(spell->toJson());
    }
    return j;
}

void PlayerHand::fromJson(const json& j) {
    spells.clear();
    for (const auto& spell_json : j) {
        BasicSpell::Type type = spell_json.at("type").get<BasicSpell::Type>();
        std::unique_ptr<BasicSpell> newSpell;
        if (type == BasicSpell::Type::Trap) {
            newSpell = std::make_unique<TrapSpell>(type, 0, 0, "");
        } else {
            newSpell = std::make_unique<DamagingSpell>(type, 0, 0, "");
        }

        newSpell->fromJson(spell_json);

        spells.push_back(std::move(newSpell));
    }
}