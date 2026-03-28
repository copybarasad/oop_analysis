#pragma once
#include "managment/game.h"
#include <iostream>
#include <vector>
#include <string>

class ConsoleRenderer {

public:
    void drawField(const Field& field);

    void drawPlayer(const Player& player);

    void drawEnemies(const std::vector<Position> &enemies);

    void drawLevel(int level);

    void drawGameOver(const Player& player);

    void drawHand(const Hand& hand);

    void drawImproveOptions();

    void showStartMenu();

    void drawSaveDir(std::string folderPath);

    void drawSave();
    
};
