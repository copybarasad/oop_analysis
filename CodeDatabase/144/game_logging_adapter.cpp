 #include "game.h"

 void Game::logEvent(GameEventType type, const std::string& message) {
 	if (eventBus.hasSink()) {
 		eventBus.publish(GameEvent{type, message});
 	}
 }

