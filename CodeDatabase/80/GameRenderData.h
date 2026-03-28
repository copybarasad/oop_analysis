#pragma once
#ifndef GAMERENDERDATA_H
#define GAMERENDERDATA_H

#include <string>
#include <vector>

class Map;

struct GameRenderData {
    int currentLevel = 1;
    int turnCounter = 0;
    bool gameRunning = false;

    bool inMainMenu = false;
    bool inLoadMenu = false;
    bool inSaveMenu = false;
    bool levelCompleted = false;
    bool gameOver = false;
    bool victory = false;

    int playerHP = 100;
    int playerMaxHP = 100;
    int playerDamage = 10;
    int playerSpellCount = 3;
    int enemiesDefeated = 0;
    bool playerSlowed = false;
    int playerX = 0;
    int playerY = 0;

    Map* gameMap = nullptr;
    int mapWidth = 20;
    int mapHeight = 20;

    std::string statusMessage;
    std::string inputPrompt;

    std::vector<std::string> saveList;
    std::vector<std::string> menuOptions;

    GameRenderData() = default;
};

#endif 