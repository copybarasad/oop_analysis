#include "../../Headers/Containers/CompositionRootN1.h"

CompositionRootN1::CompositionRootN1(int width, int height) :
	_container{ width, height }, _movableController{ &_movableContainer, &_container },
	_playerCreator{ &_movableController, &_container, &_mortalContainer, &_damagerContainer },
	_enemyCreator{ &_movableController, &_container, &_mortalContainer, &_damagerContainer }
{
}

CompositionRootN1::CompositionRootN1(int width, int height, std::vector<Cell>& walls) :
	_container{ width, height, walls }, _movableController{ &_movableContainer, &_container },
	_playerCreator{ &_movableController, &_container, &_mortalContainer, &_damagerContainer },
	_enemyCreator{ &_movableController, &_container, &_mortalContainer, &_damagerContainer }
{
}
