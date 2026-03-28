#include "SaveManager.h"
#include "Exceptions.h"
#include <fstream>
#include <iostream>

void SaveManager::save(const std::string& filename, int level, const Field& field, const EntityManager& entity_manager) const {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
        throw FileOpenException(filename);
    }
    
    ofs << level << "\n";
    
    field.save(ofs);
    
    Player* p = entity_manager.getPlayer();
    if(p) {
        ofs << 1 << "\n"; 
        p->save(ofs);
    } else {
        ofs << 0 << "\n";
    }

    auto saveEntities = [&](const auto& container) {
        ofs << container.size() << "\n";
        for(const auto& e : container) e->save(ofs);
    };

    saveEntities(entity_manager.getEnemies());
    saveEntities(entity_manager.getBuildings());
    saveEntities(entity_manager.getTowers());
    saveEntities(entity_manager.getTraps());

    ofs << 0 << "\n"; 
}

void SaveManager::load(const std::string& filename, int& level, Field& field, EntityManager& entity_manager) const {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw FileOpenException(filename);
    }
    
    entity_manager.clearAll();
    
    if (!(ifs >> level)) {
        throw FileFormatException(filename, "Could not read current level.");
    }
    
    field.load(ifs);
    
    int playerExists;
    if (!(ifs >> playerExists)) throw FileFormatException(filename, "Error reading player flag.");

    if(playerExists) {
        entity_manager.createPlayer(0,0,0,0,0);
        entity_manager.getPlayer()->load(ifs);
    }

    auto loadEntities = [&](const auto& loader_factory, const std::string& entityName) {
        int count;
        if (!(ifs >> count)) throw FileFormatException(filename, "Error reading count for " + entityName);
        for(int i=0; i<count; ++i) {
            loader_factory();
        }
    };

    loadEntities([&]() {
        auto e = std::make_unique<Enemy>(0,0,0,0);
        e->load(ifs);
        entity_manager.addEnemy(std::move(e));
    }, "Enemies");

    loadEntities([&]() {
        auto b = std::make_unique<EnemyBuilding>(0,0,0,0);
        b->load(ifs);
        entity_manager.addBuilding(std::move(b));
    }, "Buildings");

    loadEntities([&]() {
        auto t = std::make_unique<EnemyTower>(0,0,0,0,0,0);
        t->load(ifs);
        entity_manager.addTower(std::move(t));
    }, "Towers");

    loadEntities([&]() {
        auto t = std::make_unique<Trap>(0,0,0);
        t->load(ifs);
        entity_manager.addTrap(std::move(t));
    }, "Traps");
    
    int endMarker;
    if (!(ifs >> endMarker)) throw FileFormatException(filename, "Unexpected end of file or data corruption.");
}