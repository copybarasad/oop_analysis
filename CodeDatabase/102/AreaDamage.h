#pragma once
#include "../Containers/FieldContainer.h"
#include "../Containers/MortalContainer.h"
#include "../Entities/IDamager.h"
#include "../Entities/ISpell.h"

class AreaDamage : public ISpell, public IDamager
{
    const FieldContainer* _container;
    const MortalContainer* _mortalContainer;
    int _damage = 20;
public:
    AreaDamage(const FieldContainer* container, const MortalContainer* mortalContainer);

    void Start(Cell position) override;

    void ChangeDamageByValue(int value) override;

    int GetDamage() override;
};