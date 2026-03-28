#include <stdexcept>
#include "../../Headers/Facades/PlayerCreator.h"

PlayerCreator::PlayerCreator(MovableController* controller, FieldContainer* container, MortalContainer* mortalContainer,
                             DamagerContainer* damagerContainer) :
                             _controller{ controller }, _container{ container }, _mortalContainer{ mortalContainer },
                                                                   _damagerContainer{ damagerContainer }
{
}

void PlayerCreator::Destroy(Player* player)
{
	_damagerContainer->RemoveEntity(player);
	_controller->RemoveEntity(player);
	_mortalContainer->RemoveEntity(player);
	_container->RemoveEntity(player);
	_players.remove_if([player](Player& p) { return &p == player; });
}

Player* PlayerCreator::Create(Cell position, int speed, int maxHealth, int nearDamage, int farDamage)
{
	if (_container->GetEntity(position) != nullptr)
		throw std::runtime_error("Невозможно создать на этой позиции.");
	_players.emplace_back(_mortalContainer, position, speed, maxHealth, nearDamage, farDamage);
	Player* ptr = &_players.back();
	_container->AddEntity(ptr);
	_mortalContainer->AddEntity(ptr, ptr);
	_controller->AddEntity(ptr, ptr);
	_damagerContainer->AddEntity(ptr, ptr);
	return ptr;
}

std::list<Player> PlayerCreator::GetAll()
{
	return _players;
}
