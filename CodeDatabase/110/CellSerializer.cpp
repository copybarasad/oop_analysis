#include "CellSerializer.hpp"

CellSerializer::CellSerializer (const Cell& cell) :
    cell(cell) {}

std::string CellSerializer::serialize() const {
    std::ostringstream oss;
    oss << cell.getX() << "," << cell.getY() << ",";
    
    switch (cell.getType()) {
        case CellType::NORMAL: oss << "NORMAL"; break;
        case CellType::BLOCKED: oss << "BLOCKED"; break;
        case CellType::SLOWED: oss << "SLOWED"; break;
    }
    
    oss << ",";
    
    if (!cell.isEmpty()) {
        const Entity& entity = cell.getEntity();
        EntitySerializer entitySerializer(entity);
        std::string entityData = entitySerializer.serialize();
        std::replace(entityData.begin(), entityData.end(), ',', ';');
        oss << entityData;
    }
    else {
        oss << "EMPTY";
    }
    
    return oss.str();
}