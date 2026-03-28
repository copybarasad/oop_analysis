#pragma once
#include "../Containers/UpdatingContainer.h"

class UpdatingController
{
    UpdatingContainer* _container;
public:
    explicit UpdatingController(UpdatingContainer* container);

    void Update();

    bool AddEntity(IUpdating* updating);

    bool RemoveEntity(IUpdating* updating);
};