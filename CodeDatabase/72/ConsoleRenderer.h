#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include "../field_objects/Field.h"
#include <iostream>
#include <cstdlib>

class ConsoleRenderer {
    static constexpr const char* PLAYER = "P ";
    static constexpr const char* ENEMY = "E ";
    static constexpr const char* OBSTACLE = "O ";
    static constexpr const char* ENEMY_BUILD = "B ";
    static constexpr const char* SLOW_CELL = "~ ";
    static constexpr const char* EMPTY_CELL = ". ";
    static constexpr const char* TRAP = "T ";
    static constexpr const char* TOWER = "I ";

    void printColored(const std::string &text, const std::string &color);

public:
    void drawField(const Field& field);

    void drawMessage(const std::string &message);

    void drawMessage(const std::string& title, const std::string& message);

    void drawSaveMessage(const std::string& filename);

    void drawGameOver(bool won);

    void drawLoadMenu(const std::vector<std::string>& saveFiles, int selectedIndex);

    void drawMainMenu(const std::vector<std::string>& menuItems, int selectedIndex);
};

#endif // CONSOLE_RENDERER_H