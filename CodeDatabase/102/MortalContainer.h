#pragma once
#include "../Abstractions/PositionEntityContainer.h"
#include "../Entities/IMortal.h"

class MortalContainer : public PositionEntityContainer<IMortal>
{
};