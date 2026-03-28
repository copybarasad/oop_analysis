#include "Serialization/JsonManager.hpp"

void JsonManager::marshal(const HandDTO& hand, const char* path) {
    nlohmann::json j;

    j["targetLvl"] = hand.targetLvl;
    j["areaLvl"] = hand.areaLvl;
    j["trapLvl"] = hand.trapLvl;
    j["capacity"] = hand.capacity;

    j["spells"] = nlohmann::json::array();

    for (const auto& spell : hand.spells) {
        switch (spell) {
            case SpellType::TargetSpell:
                j["spells"].push_back("TargetSpell");
                break;
            case SpellType::AoESpell:
                j["spells"].push_back("AoESpell");
                break;
            case SpellType::TrapSpell:
                j["spells"].push_back("TrapSpell");
                break;
        }
    }

    fileManager.write(path, j);
}
