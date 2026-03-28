#pragma once
#include "StraightDamage.h"
#include "../Containers/MortalContainer.h"
#include "../Entities/Movable.h"
#include "../Entities/IDamager.h"
#include "../Entities/IUpdating.h"

class StraightTower : public PositionEntity, public IDamager, public IUpdating
{
    const FieldContainer* _container;
    const MortalContainer* _mortalContainer;
    StraightDamage* _spell;
    std::vector<IMortal*> _victims;
    int _stepCount = 0;
public:
    struct StraightTowerData
    {
        Cell _position;
        int _damage;

        explicit StraightTowerData(const StraightTower* tower);

        explicit StraightTowerData(std::string& s, int index);

        std::string ToString(int index) const;
    };

    StraightTower(const FieldContainer* container, const MortalContainer* mortalContainer,
        StraightDamage* spell, Cell position);

    void ChangeDamageByValue(int value) override;

    void Update() override;

    int GetDamage() override;

    void AddVictim(IMortal* victim);

    StraightDamage* GetSpell();
};
