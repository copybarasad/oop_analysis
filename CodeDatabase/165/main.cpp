#include <iostream>
#include "map.h"
#include <random>
#include <functional>
#include <iostream>
#include "./game/gameManager.h"
#include "./abstractNinterfaces/defines.h"
int main(){
    game* gameObj = new game();
    auto* manager = new gameManager<consoleReader>();
    std::cout << HELLOPLAYERTEXT << MENU;
    while(gameObj->isActive()){
        manager->sendCommand(gameObj);
    }
    delete gameObj;
    delete manager;
}