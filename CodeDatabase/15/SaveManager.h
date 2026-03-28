#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <string>
#include "Field.h"
#include "EntityManager.h"

class SaveManager {
public:
    void save(const std::string& filename, int level, const Field& field, const EntityManager& entity_manager) const;

    void load(const std::string& filename, int& level, Field& field, EntityManager& entity_manager) const;
};

#endif