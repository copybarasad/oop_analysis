#include <iostream>
#include "../../Headers/Realizations/StraightDamage.h"

StraightDamage::StraightDamage(const FieldContainer* container, const MortalContainer* mortalContainer, int maxDistance)
    : _container{ container }, _mortalContainer{ mortalContainer }, _maxDistance{ maxDistance }
{
}

void StraightDamage::Start(Cell position)
{
    if (!_target)
    {
        std::cout << "Активация прямого урона невозможна: отсутствует цель." << std::endl;
        return;
    }
    if (_container->GetDistance(position, _targetEntity->GetPosition()) > _maxDistance)
    {
        std::cout << "Активация прямого урона невозможна: превышение максимальной дистанции." << std::endl;
        return;
    }
    std::cout << "Прямой урон активирован." << std::endl;
    _target->TakeDamage(_damage);
}

void StraightDamage::SetTarget(PositionEntity* entity)
{
    if (IMortal* mortal = _mortalContainer->GetEntity(entity))
    {
        std::cout << "Цель для прямого урона выбрана." << std::endl;
        _target = mortal;
        _targetEntity = entity;
    }
    else
        std::cout << "Цель для прямого урона не может быть неуязвимой." << std::endl;
}

void StraightDamage::ChangeDamageByValue(int value)
{
    _damage = value;
}

int StraightDamage::GetDamage()
{
    return _damage;
}

void StraightDamage::DeleteTarget()
{
    _target = nullptr;
    _targetEntity = nullptr;
}

int StraightDamage::GetMaxDistance()
{
    return _maxDistance;
}
