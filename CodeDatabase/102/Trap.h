#pragma once
#include "../Containers/FieldContainer.h"
#include "../Containers/MortalContainer.h"
#include "../Controllers/UpdatingController.h"
#include "../Entities/IDamager.h"
#include "../Entities/ISpell.h"

class Trap : public ISpell, public IDamager, public IUpdating
{
    const FieldContainer* _container;
    const MortalContainer* _mortalContainer;
    UpdatingController* _updater;
    Cell _position;
    int _damage = 40;
    bool _isFinished = false;
public:
    Trap(const FieldContainer* container, const MortalContainer* mortalContainer,
        UpdatingController* updater);

    void Start(Cell position) override;

    void ChangeDamageByValue(int value) override;

    void Update() override;

    int GetDamage() override;

    Cell GetPosition();

    bool IsFinished();
};