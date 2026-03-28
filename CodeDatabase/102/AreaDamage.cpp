#include <iostream>
#include "../../Headers/Realizations/AreaDamage.h"

AreaDamage::AreaDamage(const FieldContainer* container, const MortalContainer* mortalContainer) :
    _container{ container }, _mortalContainer{ mortalContainer }
{
}

void AreaDamage::Start(Cell position)
{
    std::cout << "Урон по площади активирован. Квадрат действия: (" << position._x << "; " << position._y <<
        "), (" << position._x + 1 << "; " << position._y << "), (" << position._x << "; " << position._y + 1 <<
            "), (" << position._x + 1 << "; " << position._y + 1 << ")." << std::endl;
    IMortal* mortals[4]
    {
        _mortalContainer->GetEntity(_container->GetEntity(position)),
        _mortalContainer->GetEntity(_container->GetEntity(Cell(position._x + 1, position._y))),
        _mortalContainer->GetEntity(_container->GetEntity(Cell(position._x, position._y + 1))),
        _mortalContainer->GetEntity(_container->GetEntity(Cell(position._x + 1, position._y + 1)))
    };
    for (IMortal* mortal : mortals)
    {
        if (mortal != nullptr)
            mortal->TakeDamage(_damage);
    }
}

void AreaDamage::ChangeDamageByValue(int value)
{
    _damage = value;
}

int AreaDamage::GetDamage()
{
    return _damage;
}
