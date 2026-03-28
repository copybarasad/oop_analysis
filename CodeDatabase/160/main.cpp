#include "../include/core/game_controller.h"
#include "../include/core/game_state.h"
#include "../include/io/console_command_reader.h"
#include "../include/ui/game_visualizer.h"
#include "../include/ui/renderer.h"

int main() {
	GameState game(25, 25);
	GameVisualizer<Renderer> visualizer;
	GameController<ConsoleCommandReader> controller(game);

	controller.Run(visualizer);
	return 0;
}
