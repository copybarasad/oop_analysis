#include "EntityManagerDeserializer.hpp"

EntityManager* EntityManagerDeserializer::deserialize (const std::string& data, Field* field) {
    EntityManager* manager = new EntityManager();
    std::istringstream stream(data);
    std::string line;
    
    while (std::getline(stream, line)) {
        if (line == "entities:") {
            parseEntities(stream, manager, field);
        }
    }
    
    return manager;
}

void EntityManagerDeserializer::parseEntities (std::istringstream& stream, EntityManager* manager, Field* field) {
    std::string line;
    while (std::getline(stream, line) && !line.empty() && line != "typeCounters:") {
        auto parts = split(line, ':');
        if (parts.size() == 2) {
            std::string entityId = parts[0];
            auto coords = split(parts[1], ',');
            if (coords.size() == 2) {
                unsigned int y = parseUnsignedInt(coords[0], "entity y");
                unsigned int x = parseUnsignedInt(coords[1], "entity x");
                
                Cell& cell = field->getCell(x, y);
                if (!cell.isEmpty()) {
                    manager->addTrack(&cell);
                }
                else {
                    throw ValidationException("Entity " + entityId + " references empty cell at " + std::to_string(x) + "," + std::to_string(y));
                }
            }
        }
    }
}