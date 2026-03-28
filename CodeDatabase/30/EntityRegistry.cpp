#include "../include/EntityRegistry.h"
#include <iostream>

EntityId EntityRegistry::create(std::unique_ptr<Entity> e) {
    if (!e) return InvalidEntityId;
    EntityId id;
    if (!free_.empty()) {
        id = free_.back();
        free_.pop_back();
    } else {
        id = nextId_++;
    }
    storage_[id] = std::move(e);
    return id;
}

void EntityRegistry::destroy(EntityId id) {
    if (id == InvalidEntityId) return;
    auto it = storage_.find(id);
    if (it != storage_.end()) {
        storage_.erase(it);
        free_.push_back(id);
    }
}

Entity* EntityRegistry::get(EntityId id) {
    auto it = storage_.find(id);
    return it == storage_.end() ? nullptr : it->second.get();
}

const Entity* EntityRegistry::get(EntityId id) const {
    auto it = storage_.find(id);
    return it == storage_.end() ? nullptr : it->second.get();
}

bool EntityRegistry::alive(EntityId id) const {
    return storage_.find(id) != storage_.end();
}

size_t EntityRegistry::size() const {
    return storage_.size();
}

void EntityRegistry::restore(EntityId id, std::unique_ptr<Entity> e) {
    if (!e) return;
    storage_[id] = std::move(e);
}

void EntityRegistry::saveState(std::ostream& os) const {
    os << nextId_ << " " << free_.size() << "\n";
    for (EntityId id : free_) {
        os << id << " ";
    }
    os << "\n";
}

void EntityRegistry::loadState(std::istream& is) {
    storage_.clear();
    free_.clear();
    size_t freeCount;
    is >> nextId_ >> freeCount;
    for (size_t i = 0; i < freeCount; ++i) {
        EntityId id;
        is >> id;
        free_.push_back(id);
    }
}
