#pragma once
#include "Renderer.h"

class ConsoleRenderer : public Renderer {
public:
    void render(const GameCycle& game) override;

    static void clearScreen();
    static int showMainMenu();
    static void getFieldSize(int& rows, int& cols);
    static std::string getLoadFilename();
};