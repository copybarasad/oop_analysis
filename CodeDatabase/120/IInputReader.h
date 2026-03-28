#pragma once
#include <optional>
#include <vector>
#include "Position.h"
#include <memory>
class ISpell;

class IInputReader {
public:
	virtual ~IInputReader() = default;
	virtual char GetCommandChar() = 0;
	virtual std::optional<size_t> GetSpellIndex(const std::vector<std::unique_ptr<ISpell>>& spells) = 0;
	virtual std::optional<Position> GetTargetPosition() = 0;
	virtual std::optional<int> GetShopChoice(int playerScore) = 0;
	virtual bool WantsToSave() = 0;
	virtual bool WantsToLoad() = 0;
};
