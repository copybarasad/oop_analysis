#include "../../Headers/Controllers/UpdatingController.h"

UpdatingController::UpdatingController(UpdatingContainer* container) : _container{ container }
{
}

void UpdatingController::Update()
{
    for (IUpdating* updating : _container->GetAll())
        updating->Update();
}

bool UpdatingController::AddEntity(IUpdating* updating)
{
    return _container->AddEntity(updating);
}

bool UpdatingController::RemoveEntity(IUpdating* updating)
{
    return _container->RemoveEntity(updating);
}
