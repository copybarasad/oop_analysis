#ifndef ENTITY_M_DESER_H
#define ENTITY_M_DESER_H

#include "../BaseDeserializer/BaseDeserializer.hpp"
#include "modules/Managers/EntityManager/EntityManager.hpp"
#include "modules/Field/Field/Field.hpp"

class EntityManagerDeserializer : public BaseDeserializer {
    public:
    EntityManager* deserialize (const std::string& data, Field* field);

    private:
    void parseEntities (std::istringstream& stream, EntityManager* manager, Field* field);
};

#endif