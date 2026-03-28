#include "../../Headers/Containers/CompositionRootN2.h"

CompositionRootN2::CompositionRootN2(CompositionRootN1& root1) :
	_root1{ root1 }, _updater{ &_updatingContainer },
	_playerHand{ 3 }, _spellCreator{ &_updater, &root1._container, &root1._mortalContainer },
	_towerCreator{ &_updater, &root1._container, &root1._mortalContainer, &root1._damagerContainer, &_spellCreator }
{
}