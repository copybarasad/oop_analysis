#include "TrapSpell.h"

json TrapSpell::toJson() const {
    json j = BasicSpell::toJson();
    j["coordinates"] = coordinates.toJson();
    return j;
}

void TrapSpell::fromJson(const json& j) {
    BasicSpell::fromJson(j);

    if (j.contains("coordinates")) {
        coordinates.fromJson(j.at("coordinates"));
    }
}