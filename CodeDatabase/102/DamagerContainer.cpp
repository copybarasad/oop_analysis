#include "../../Headers/Containers/DamagerContainer.h"

bool DamagerContainer::AddEntity(PositionEntity* entity, ISwitchingDamager* typed)
{
    _entities[entity] = typed;
    return PositionEntityContainer::AddEntity(entity, typed);
}

bool DamagerContainer::RemoveEntity(PositionEntity* entity)
{
    _entities.erase(entity);
    return PositionEntityContainer::RemoveEntity(entity);
}