#ifndef ENTITY_MANAGER_SER_H
#define ENTITY_MANAGER_SER_H

#include "../../BaseSerializable/Serializable.hpp"
#include "../../../../../Managers/EntityManager/EntityManager.hpp"

class EntityManagerSerializer : public Serializable {
    private:
    EntityManager& manager;
    
    public:
    EntityManagerSerializer (EntityManager& manager);
    
    std::string serialize () const override;
};

#endif