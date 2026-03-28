#pragma once
#include <unordered_map>
#include "../Entities/PositionEntity.h"

template<typename T>
requires std::is_class_v<T>
class PositionEntityContainer
{
    std::unordered_map<PositionEntity*, T*> _entities;
public:
    virtual bool AddEntity(PositionEntity* entity, T* typed);

    virtual bool RemoveEntity(PositionEntity* entity);

    T* GetEntity(PositionEntity* entity) const;
};