#include "SaveCommand.h"
#include "SaveManager.h"
#include "ExceptionHandler.h"

bool SaveCommand::Execute(GameContext& context) {
	if (!context.input_reader.WantsToSave()) {
		context.events.Notify(GameMessageEvent{"Save canceled by player."});
		return false;
	}

	std::string filename;
	std::cout << "Enter save filename: ";
	std::cin >> filename;

	SaveManager saver;
	ExceptionHandler handler(&context.logic);
	return handler.Handle([&]() -> bool {
		saver.SaveGame(filename, context.world);
		context.events.Notify(GameMessageEvent{"Game saved successfully."});
		return true;
	});
}
