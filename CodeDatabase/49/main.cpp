#include "game/game.h"
#include "game/game_controller.h"
#include "commands/console_input_reader.h"
#include "render/game_visualizer.h"
#include "render/console_renderer.h"
#include "logging/game_logger.h"
#include "logging/event_manager.h"

int main()
{
	GameLogger logger(LogOutput::FILE, "logs/game.log");
	EventManager::get_instance().add_listener(&logger);

	Game game(16, 16);
	GameController<ConsoleInputReader> controller(game);
	GameVisualizer<ConsoleRenderer> visualizer(game);
	game.run(controller, visualizer);

	EventManager::get_instance().remove_listener(&logger);

	return 0;
}