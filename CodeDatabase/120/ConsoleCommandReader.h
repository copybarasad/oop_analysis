#pragma once
#include "IInputReader.h"
#include <iostream>
#include <iostream>
class ConsoleCommandReader : public IInputReader {
public:
    using Mapping = std::vector<std::pair<char, std::string>>;

    ConsoleCommandReader(const Mapping& commandMap) : commandMap_(commandMap) {}
	char GetCommandChar() override;
	std::optional<size_t> GetSpellIndex(const std::vector<std::unique_ptr<ISpell>>& spells) override;
	std::optional<Position> GetTargetPosition() override;
	std::optional<int> GetShopChoice(int playerScore) override;
	bool WantsToSave() override;
	bool WantsToLoad() override;
private:
    const Mapping& commandMap_;	
};
