#include "Commands.h"

LoadCommand::LoadCommand(PlayerConfig& config, std::string& name)
	: dataAboutPlayer(config), fileName(name) {
}

void LoadCommand::Execute(GameLogic& logic)
{
	std::string fullPath = "saves/" + fileName + ".json";

	if (!std::filesystem::exists("saves"))
		throw FileException("The game hasn't been saved yet.");

	std::ifstream save(fullPath);

	if (!save.is_open())
		throw FileException(std::format("File with name {} cannot open for loading.\n", fileName));

	json j;
	std::string encryptedData;

	try
	{
		save >> encryptedData;

		for (auto i = 0; i < encryptedData.size(); ++i)
			encryptedData[i] ^= ENCRYPTION_KEY;

		j = json::parse(encryptedData);

		JsonSerializer::Deserialize(dataAboutPlayer, logic, j);
	}
	catch (const json::parse_error& ex)
	{
		throw FileException(std::format("Json parse error: {}", ex.what()));
	}
}