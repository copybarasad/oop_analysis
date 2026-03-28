#include "LoadCommand.h"
#include "LoadManager.h"

bool LoadCommand::Execute(GameContext& context) {
	if (!context.input_reader.WantsToLoad()) {
		context.events.Notify(GameMessageEvent{"Load canceled by player."});
		return false;
	}

	std::string filename;
	std::cout << "Enter save filename to load: ";
	std::cin >> filename;

	LoadManager loader;
	ExceptionHandler handler(&context.logic);
	return handler.Handle([&]() -> bool {
		loader.LoadGame(filename, context.world);
		context.events.Notify(GameMessageEvent{"Game loaded successfully."});
		return true;
	});
}
