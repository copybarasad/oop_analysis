#define SDL_MAIN_HANDLED

#include <filesystem>
#include "core/Game.h"
#include "core/GameSave.h"

int main() {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }
    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << "\n";
        return 1;
    }

    std::filesystem::path saveFile = std::filesystem::path(std::getenv("APPDATA")) / "WarriorAdventure" / "save.json";
    std::filesystem::path controlsFile = std::filesystem::path("controls.json");

    Game game = Game(saveFile, controlsFile);


    GameLogger logger(LoggingMode::File, "log.txt");

    game.event_handler().subscribe(&logger);

    if (!game.init("Warrior Adventure")) {
        return 1;
    }
    game.run();

    return 0;
}
