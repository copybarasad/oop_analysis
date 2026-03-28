#include "EntityManager.hpp"

#include <iostream>

EntityManager::EntityManager () :       // 0 - player, 100 - ally, 200 - trap, 300 - enemy, 400 - building, 500 - tower
    entityCounter(0) {
    for (size_t i = 0; i < 600; i += 100) {
        typeCounter[i] = 0;
    }
}

void EntityManager::addTrack (Cell* trackingCell) {
    size_t newID = trackingCell->getEntity().getID() + typeCounter[trackingCell->getEntity().getID()]++;
    trackingCell->getEntity().setID(newID);
    entityTracker[newID] = trackingCell;
}

void EntityManager::removeTrack (size_t id) {
    delete &entityTracker[id]->getEntity();
    entityTracker.extract(id);
    typeCounter[id / 100 * 100]--;
}

std::map<size_t, Cell*>& EntityManager::getTracker () { return entityTracker; }

const std::map<size_t, size_t>& EntityManager::getTypeCounter() const { return typeCounter; }

Cell& EntityManager::getPlayerPos () { return *entityTracker[0]; }

size_t EntityManager::getEntityNumber () const {
    size_t num = 0;
    for (auto& count: typeCounter) {
        num += count.second;
    }
    return num;
}

size_t EntityManager::getEnemiesNumber () const {
    size_t num = 0;
    for (auto& count : typeCounter) {
        if (count.first >= 300 && count.first < 600){
            num += count.second;
        }
    }
    return num;
}

void EntityManager::updateTrack (size_t id, Cell& trackingCell) {
    entityTracker[id] = &trackingCell;
}

void EntityManager::deleteEntities () {
    for (auto& track: entityTracker) {
        delete &track.second->getEntity();
    }
    entityTracker.clear();
}