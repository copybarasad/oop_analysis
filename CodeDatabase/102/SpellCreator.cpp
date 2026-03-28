#include <string>
#include "../../Headers/Facades/SpellCreator.h"

SpellCreator::SpellCreator(UpdatingController* updater, FieldContainer* fieldContainer,
    MortalContainer* mortalContainer) : _updater{ updater }, _fieldContainer{ fieldContainer },
    _mortalContainer{ mortalContainer }
{
    _spellCreations =
    {
        {
            "StraightDamage", [this]
            {
                _straightDamages.emplace_back(_fieldContainer, _mortalContainer, 3);
                return &_straightDamages.back();
            }
        },
        {
            "AreaDamage", [this]
            {
                _areaDamages.emplace_back(_fieldContainer, _mortalContainer);
                return &_areaDamages.back();
            }
        },
        {
            "Trap", [this]
            {
                _traps.emplace_back(_fieldContainer, _mortalContainer, _updater);
                return &_traps.back();
            }
        }
    };
    updater->AddEntity(this);
}

void SpellCreator::Destroy(ISpell* spell)
{
    _straightDamages.remove_if([spell] (StraightDamage& s) { return &s == spell; });
    _areaDamages.remove_if([spell] (AreaDamage& a) { return &a == spell; });
    _traps.remove_if([spell, this] (Trap& t)
    {
        if (&t == spell)
        {
            _updater->RemoveEntity(&t);
            return true;
        }
        return false;
    });
}

void SpellCreator::Update()
{
    for(Trap& t : _traps)
    {
        if(t.IsFinished())
            _traps.remove_if([t] (Trap& t1){return &t1 == &t;});
    }
}

ISpell* SpellCreator::Create(const std::string& name)
{
    if (_spellCreations.contains(name))
        return _spellCreations[name]();
    return nullptr;
}