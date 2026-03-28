#include "../../Headers/Abstractions/PositionEntityContainer.h"

template<typename T>
requires std::is_class_v<T>
bool PositionEntityContainer<T>::AddEntity(PositionEntity *entity, T* typed)
{
    if (_entities.contains(entity))
        return false;
    _entities[entity] = typed;
    return true;
}

template<typename T>
requires std::is_class_v<T>
bool PositionEntityContainer<T>::RemoveEntity(PositionEntity* entity)
{
    if (!_entities.contains(entity))
        return false;
    _entities.erase(entity);
    return true;
}

template<typename T>
requires std::is_class_v<T>
T* PositionEntityContainer<T>::GetEntity(PositionEntity* entity) const
{
    if (_entities.contains(entity))
        return _entities.at(entity);
    return nullptr;
}