#include <stdexcept>
#include "../../Headers/Facades/EnemyCreator.h"

EnemyCreator::EnemyCreator(MovableController* controller, FieldContainer* container, MortalContainer* mortalContainer, 
                           DamagerContainer* damagerContainer) : _controller{ controller },
                                                                 _container{container}, _mortalContainer {mortalContainer},
                                                                 _damagerContainer{ damagerContainer }
{
}

void EnemyCreator::Destroy(Enemy* enemy)
{
	_damagerContainer->RemoveEntity(enemy);
	_controller->RemoveEntity(enemy);
	_mortalContainer->RemoveEntity(enemy);
	_container->RemoveEntity(enemy);
	_enemies.remove_if([enemy](Enemy& e) { return &e == enemy; });
}

Enemy* EnemyCreator::Create(Cell position, int speed, int maxHealth, int damage)
{
	if (_container->GetEntity(position) != nullptr)
		throw std::runtime_error("Невозможно создать на этой позиции.");
	_enemies.emplace_back(_mortalContainer, position, speed, maxHealth, damage);
	Enemy* ptr = &_enemies.back();
	_container->AddEntity(ptr);
	_mortalContainer->AddEntity(ptr, ptr);
	_controller->AddEntity(ptr, ptr);
	_damagerContainer->PositionEntityContainer<IDamager>::AddEntity(ptr, ptr);
	return ptr;
}

std::list<Enemy> EnemyCreator::GetAll()
{
	return _enemies;
}
