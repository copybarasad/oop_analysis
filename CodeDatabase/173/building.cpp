#include "building.hpp"
#include "spawner-building.hpp"
#include "spell-building.hpp"

Building::~Building() {}

Building *Building::fromByteDump(bytedump bytes) {
    if (bytes.size() != 16) {
        throw std::length_error("building dump must be 16 bytes long");
    }

    int buildingType = static_cast<int>(bytes[0]);

    switch (buildingType) {
    case SPAWNER_BUILDING_BYTE:
        return new SpawnerBuilding(bytes);
    case SPELL_BUILDING_BYTE:
        return new SpellBuilding(bytes);
    default:
        throw std::logic_error("unknown building type");
    }
}
