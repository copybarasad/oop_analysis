#ifndef MENU_CONTROLLER_HPP
#define MENU_CONTROLLER_HPP

#include "game_controller.hpp"
#include <string>
#include <vector>
#include <functional>

class MenuController {
private:
    GameController gameController;
    
    char getCharacter() const;
    void waitForAnyKey() const;
    void showMessage(const std::string& message) const;
    
    void showMenu(const std::string& title, 
                  const std::vector<std::pair<char, std::string>>& options,
                  std::function<void(char)> handler);
    
    void showHelp() const;
    void showSaveMenu() const;
    void showLoadMenu();
    
public:
    void runMainMenu();
    void showLevelCompleteMenu();
    void showGameOverMenu();
};

#endif