#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <string>
#include <vector>
#include <memory>


class Game;

class GameMenu {
public:
    enum MenuOption {
        NEW_GAME = 1,
        LOAD_GAME,
        EXIT
    };

    static void run();
    
    static MenuOption showMainMenu();
    static MenuOption showGameOverMenu();
    static std::string showSaveGameMenu();
    static std::string showLoadGameMenu();
    
private:
    static std::vector<std::string> getSaveFiles();
    
    static void handleMainMenu();
    static void handleGameOverMenu();
    static void runGame(std::unique_ptr<Game> game);
};

#endif