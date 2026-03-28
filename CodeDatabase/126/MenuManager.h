#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <iostream>

class MenuManager {
public:
    void showMainMenu();
    int getMenuChoice();
    void handleMainMenu();
    
private:
    void startNewGame();
    void loadGame();
};

#endif