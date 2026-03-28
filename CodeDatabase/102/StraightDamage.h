#pragma once
#include "../Containers/FieldContainer.h"
#include "../Containers/MortalContainer.h"
#include "../Entities/IDamager.h"
#include "../Entities/ITargetSpell.h"

class StraightDamage : public ITargetSpell, public IDamager
{
    const FieldContainer* _container;
    const MortalContainer* _mortalContainer;
    IMortal* _target = nullptr;
    PositionEntity* _targetEntity = nullptr;
    int _maxDistance;
    int _damage = 40;
public:
    StraightDamage(const FieldContainer* container, const MortalContainer* mortalContainer, int maxDistance);

    void Start(Cell position) override;

    void SetTarget(PositionEntity* entity) override;

    void ChangeDamageByValue(int value) override;

    int GetDamage() override;

    void DeleteTarget();

    int GetMaxDistance();
};