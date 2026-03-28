#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <string>

class InputManager {
public:
    static char getInput();
    static int getFieldSize(const std::string& dimension);
    static int getEnemyCount();
    static void waitForEnter();
};

#endif