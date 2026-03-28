#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <string>
#include "Game.h"

class ControlConfig;
class GameEventObserver;

class GameController {
public:
    GameController();  
    ~GameController();  
    void start(); 
    bool loadGame(const std::string& filename);  
    void saveGame(const std::string& filename);  
    void startNewGame();  
    void showMainMenu();  
    void setControlsFile(const std::string& filename);  
    void setLoggingMode(const std::string& mode);  

private:
    Game* gamePtr;  
    ControlConfig* controlConfig_;  
    GameEventObserver* eventObserver_;  
    void displaySaveFiles(); 
    std::vector<std::string> listSaveFiles(const std::string& dir = ".");  // Получить список сохранений
};

#endif
