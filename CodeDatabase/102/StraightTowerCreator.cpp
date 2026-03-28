#include <stdexcept>
#include "../../Headers/Facades/StraightTowerCreator.h"

StraightTowerCreator::StraightTowerCreator(UpdatingController* updater, FieldContainer* container,
                                           MortalContainer* mortalContainer, DamagerContainer* damagerContainer, SpellCreator* spellCreator)
	: _updater{ updater }, _container{ container }, _mortalContainer{ mortalContainer },
	_damagerContainer{ damagerContainer }, _spellCreator{ spellCreator }
{
}

void StraightTowerCreator::Destroy(StraightTower* tower)
{
	_updater->RemoveEntity(tower);
	_damagerContainer->RemoveEntity(tower);
	_container->RemoveEntity(tower);
	_spellCreator->Destroy(tower->GetSpell());
	_towers.remove_if([tower](StraightTower& e) { return &e == tower; });
}

StraightTower* StraightTowerCreator::Create(Cell position, int damage)
{
	if (_container->GetEntity(position) != nullptr)
		throw std::runtime_error("Невозможно создать на этой позиции.");
	StraightDamage* damageSpell = static_cast<StraightDamage*>(_spellCreator->Create("StraightDamage"));
	damageSpell->ChangeDamageByValue(damage);
	_towers.emplace_back(_container, _mortalContainer, damageSpell, position);
	StraightTower* ptr = &_towers.back();
	_container->AddEntity(ptr);
	_damagerContainer->PositionEntityContainer<IDamager>::AddEntity(ptr, ptr);
	_updater->AddEntity(ptr);
	return ptr;
}

std::list<StraightTower> StraightTowerCreator::GetAll()
{
	return _towers;
}
