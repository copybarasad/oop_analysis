#pragma once
#include "Commands/Command.h"
#include "DataTypes/InputEvent.h"


class IConverter
{
public:
	virtual Command* convert(const InputEvent& event) = 0;
};

