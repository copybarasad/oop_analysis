#include "PetSerializer.h"
#include "../entities/Pet.h"
#include "EntitySerializer.h"

nlohmann::json PetSerializer::serialize(const Pet& pet) const {
    nlohmann::json json = entitySerializer.serialize(pet);
    json["type"] = "Pet";
    return json;
}

void PetSerializer::deserialize(Pet& pet, const nlohmann::json& json) const {
    if (!json.contains("type") || json["type"] != "Pet") {
        throw std::runtime_error("Некорретный тип Pet: не Pet");
    }
    entitySerializer.deserialize(pet, json);
}

