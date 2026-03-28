#include "EntityManagerSerializer.hpp"

EntityManagerSerializer::EntityManagerSerializer (EntityManager& manager) :
    manager(manager) {}

std::string EntityManagerSerializer::serialize () const {
    std::ostringstream oss;
    auto& tracker = manager.getTracker();
    
    oss << "entityCounter:" << manager.getEntityNumber() << "\n";
    oss << "enemiesNumber:" << manager.getEnemiesNumber() << "\n";
    
    oss << "entities:\n";
    for (const auto& [id, cell] : tracker) {
        oss << id << ":" << cell->getX() << "," << cell->getY() << "\n";
    }
    
    oss << "typeCounters:\n";
    auto& typeCounter = manager.getTypeCounter();
    for (const auto& [type, count] : typeCounter) {
        oss << type << ":" << count << "\n";
    }
    
    return oss.str();
}