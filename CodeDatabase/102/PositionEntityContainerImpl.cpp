#include "PositionEntityContainer.cpp"
#include "../../Headers/Entities/IDamager.h"
#include "../../Headers/Entities/IMortal.h"
#include "../../Headers/Entities/Movable.h"

template class PositionEntityContainer<IDamager>;
template class PositionEntityContainer<IMortal>;
template class PositionEntityContainer<Movable>;