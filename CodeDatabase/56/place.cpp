#include "place.h"

Place::Place() {
    effect = "no";
    Name = "...";
    type = -1;

}


int Place::getType() {
    return type;
}
nlohmann::json Place::toJson() const {
    return {
        {"type", type},
        {"effect", effect},
        {"name", Name}
    };
}

void Place::fromJson(const nlohmann::json& j) {
    type = j["type"];
    effect = j["effect"];
    Name = j["name"];
}

std::string Place::getEffect() {
    return effect;
}

std::string Place::getName() {
    return Name;
}

void Place::setEffect(std::string new_effect) {
    effect = new_effect;
}

void Place::setType(int new_type) { 
    type = new_type; 
}