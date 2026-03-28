#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "GameController.h"
#include "Position.h"
#include <memory>

class InputHandler {
public:
    static bool handleGameInput(GameController& controller, char input);
    static void showHelp();
    static Position getDirectionFromInput(char input);
};

#endif