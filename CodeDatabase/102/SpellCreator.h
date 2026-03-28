#pragma once
#include <string>
#include <functional>
#include <list>
#include <unordered_map>
#include "../Containers/FieldContainer.h"
#include "../Controllers/UpdatingController.h"
#include "../Realizations/AreaDamage.h"
#include "../Realizations/StraightDamage.h"
#include "../Realizations/Trap.h"

class SpellCreator : public IUpdating
{
    UpdatingController* _updater;
    FieldContainer* _fieldContainer;
    MortalContainer* _mortalContainer;
    std::list<StraightDamage> _straightDamages;
    std::list<AreaDamage> _areaDamages;
    std::list<Trap> _traps;
    std::unordered_map<std::string, std::function<ISpell* ()>> _spellCreations;
public:
    SpellCreator(UpdatingController* updater, FieldContainer* fieldContainer, MortalContainer* mortalContainer);

    void Destroy(ISpell* spell);

    void Update() override;

    ISpell* Create(const std::string& name);
};