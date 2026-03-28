#ifndef PETSERIALIZER_H
#define PETSERIALIZER_H

#include "ISerializer.h"
#include "EntitySerializer.h"
#include "../entities/Pet.h"

class PetSerializer : public ISerializer<Pet> {
private:
    EntitySerializer entitySerializer;
    
public:
    nlohmann::json serialize(const Pet& pet) const override;
    void deserialize(Pet& pet, const nlohmann::json& json) const override;
};

#endif

