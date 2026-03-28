#include "QuitCommand.h"

bool QuitCommand::Execute(GameContext& context) {
    (void)context;
	std::cout << "Game exited by player.\n";
	exit(0);
	return false; 
}
