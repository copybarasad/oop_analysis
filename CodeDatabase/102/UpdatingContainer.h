#pragma once
#include <vector>
#include "../Entities/IUpdating.h"

class UpdatingContainer
{
    std::vector<IUpdating*> _updates;
public:
    bool AddEntity(IUpdating* updating);

    bool RemoveEntity(IUpdating* updating);

    std::vector<IUpdating*> GetAll() const;
};