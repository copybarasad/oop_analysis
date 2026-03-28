#include "../../Headers/Containers/UpdatingContainer.h"

bool UpdatingContainer::AddEntity(IUpdating* updating)
{
    for (IUpdating* currUpdating : _updates)
    {
        if (currUpdating == updating)
            return false;
    }
    _updates.push_back(updating);
    return true;
}

bool UpdatingContainer::RemoveEntity(IUpdating* updating)
{
    for (int i = 0; i < _updates.size(); i++)
    {
        if (_updates[i] == updating)
        {
            _updates.erase(_updates.begin() + i);
            return true;
        }
    }
    return false;
}

std::vector<IUpdating*> UpdatingContainer::GetAll() const
{
    return _updates;
}
