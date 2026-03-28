#include "BasicSpell.h"
#include <format>

std::string BasicSpell::toString() {
    std::string type_str;
    switch (type) {
        case Type::Direct: type_str = "Прямой урон"; break;
        case Type::AoE: type_str = "Урон по площади"; break;
        case Type::Trap: type_str = "Ловушка"; break;
        default: break;
    }
    return std::format("{}: \n\t{}, \n\tУрон: {}, \n\tДистанция: {}\n", name, type_str, damage, range);
}

json BasicSpell::toJson() const {
    return json{
            {"type", type},
            {"damage", damage},
            {"range", range},
            {"name", name}
    };
}

void BasicSpell::fromJson(const json& j) {
    j.at("damage").get_to(damage);
    j.at("range").get_to(range);
    j.at("name").get_to(name);
}