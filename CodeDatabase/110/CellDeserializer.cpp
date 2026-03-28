#include "CellDeserializer.hpp"

Cell CellDeserializer::deserialize(const std::string& data, unsigned int x, unsigned int y) {
    auto parts = split(data, ',');
    if (parts.size() < 4) {
        throw ValidationException("Invalid cell data: " + data);
    }

    unsigned int dataY = parseUnsignedInt(parts[0], "cell y");
    unsigned int dataX = parseUnsignedInt(parts[1], "cell x");
    
    if (dataX != x || dataY != y) {
        throw ValidationException("Cell coordinate mismatch. Data: " + std::to_string(dataX) + "," + std::to_string(dataY) +
        " Expected: " + std::to_string(x) + "," + std::to_string(y));
    }

    CellType type;
    if (parts[2] == "NORMAL") {
        type = CellType::NORMAL;
    }
    else if (parts[2] == "BLOCKED") {
        type = CellType::BLOCKED;
    }
    else if (parts[2] == "SLOWED") {
        type = CellType::SLOWED;
    }
    else {
        throw ValidationException("Invalid cell type: " + parts[2]);
    }
    
    Cell cell(x, y, type);
    
    if (parts[3] != "EMPTY") {
        std::string entityData;
        for (size_t i = 3; i < parts.size(); ++i) {
            if (i > 3) entityData += ",";
            entityData += parts[i];
        }
        
        EntityDeserializer entityDeserializer;
        Entity* entity = entityDeserializer.deserialize(entityData);
        cell.setEntity(*entity);
    }
    
    return cell;
}