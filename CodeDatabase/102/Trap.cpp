#include <iostream>
#include "../../Headers/Realizations/Trap.h"

Trap::Trap(const FieldContainer* container, const MortalContainer* mortalContainer, UpdatingController* updater)
    : _container{ container }, _mortalContainer{ mortalContainer }, _updater{ updater }
{
}

void Trap::Start(Cell position)
{
    _position = position;
    _updater->AddEntity(this);
}

void Trap::ChangeDamageByValue(int value)
{
    _damage = value;
}

void Trap::Update()
{
    std::cout << "Ловушка на позиции (" << _position._x << "; " << _position._y << ")." << std::endl;
    if (IMortal* mortal = _mortalContainer->GetEntity(_container->GetEntity(_position)))
    {
        std::cout << "Ловушка активирована." << std::endl;
        mortal->TakeDamage(_damage);
        _isFinished = true;
        _updater->RemoveEntity(this);
    }
}

int Trap::GetDamage()
{
    return _damage;
}

Cell Trap::GetPosition()
{
    return _position;
}

bool Trap::IsFinished()
{
    return _isFinished;
}
