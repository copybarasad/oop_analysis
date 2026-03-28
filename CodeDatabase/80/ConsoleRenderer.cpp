#include "ConsoleRenderer.h"
#include "Map.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

ConsoleRenderer::ConsoleRenderer(int width) : screenWidth(width) {}

bool ConsoleRenderer::initialize() {
    return true;
}

void ConsoleRenderer::clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void ConsoleRenderer::render(const GameRenderData& data) {
    clearScreen();
    if (data.inMainMenu) {
        renderMainMenu(data);
        data.inMainMenu;
    }
    else if (data.inLoadMenu) {
        renderLoadMenu(data);
    }
    else if (data.inSaveMenu) {
        renderSaveMenu(data);
    }
    else if (data.levelCompleted) {
        renderLevelComplete(data);
    }
    else if (data.gameOver || data.victory) {
        renderGameEnd(data);
    }
    else {
        renderGameplay(data);
    }

    if (!data.statusMessage.empty()) {
        std::cout << "\n> " << data.statusMessage << "\n";
    }

    if (!data.inputPrompt.empty()) {
        std::cout << "\n" << data.inputPrompt;
    }

    std::cout.flush();
}

std::string ConsoleRenderer::centerText(const std::string& text, int width) const {
    int padding = (width - static_cast<int>(text.length())) / 2;
    if (padding < 0) padding = 0;
    return std::string(padding, ' ') + text;
}

void ConsoleRenderer::renderMainMenu(const GameRenderData& data) {
    std::cout << "=== MAIN MENU ===" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Exit" << std::endl;
}

void ConsoleRenderer::renderGameplay(const GameRenderData& data) {
    if (data.gameMap) {
        data.gameMap->SeeMap();
    }

    std::cout << "Level: " << data.currentLevel << std::endl;
    std::cout << "Player HP: " << data.playerHP << "/" << data.playerMaxHP << std::endl;
    std::cout << "Damage: " << data.playerDamage << std::endl;
    std::cout << "Enemies defeated: " << data.enemiesDefeated << std::endl;

    if (data.statusMessage.find("Attack mode") != std::string::npos) {
        std::cout << data.statusMessage << std::endl;
    }
}

void ConsoleRenderer::renderLevelComplete(const GameRenderData& data) {
    std::cout << "==================================" << std::endl;
    std::cout << "        LEVEL UP! LEVEL " << data.currentLevel << std::endl;
    std::cout << "==================================" << std::endl;

    if (!data.statusMessage.empty()) {
        std::cout << data.statusMessage << std::endl;
    }

    std::cout << "Choose an upgrade:" << std::endl;
    std::cout << "1. +10 Max Health (Current: " << data.playerMaxHP << ")" << std::endl;
    std::cout << "2. +5 Base Damage (Current: " << data.playerDamage << ")" << std::endl;
    std::cout << "3. Upgrade Random Spell (Spells: " << data.playerSpellCount << ")" << std::endl;
    std::cout << "4. Add New Random Spell" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Enter your choice (1-4): ";
}

void ConsoleRenderer::renderGameEnd(const GameRenderData& data) {
    if (data.victory) {
        std::cout << "==================================" << std::endl;
        std::cout << "           VICTORY!" << std::endl;
        std::cout << "==================================" << std::endl;
        std::cout << "You destroyed the enemy tower!" << std::endl;
    }
    else {
        std::cout << "==================================" << std::endl;
        std::cout << "           GAME OVER" << std::endl;
        std::cout << "==================================" << std::endl;
    }

    std::cout << "Player HP: " << data.playerHP << std::endl;
    std::cout << "Enemies defeated: " << data.enemiesDefeated << std::endl;
    std::cout << "==================================" << std::endl;
}



void ConsoleRenderer::renderLoadMenu(const GameRenderData& data) {
    std::cout << "=== LOAD GAME ===" << std::endl;

    if (!data.saveList.empty()) {
        std::cout << "Available saves:" << std::endl;
        for (size_t i = 0; i < data.saveList.size(); i++) {
            std::cout << (i + 1) << ". " << data.saveList[i] << std::endl;
        }
    }
    else {
        std::cout << "No valid save games found!" << std::endl;
        std::cout << "Make sure the 'saves' directory exists and contains .sav files." << std::endl;
    }

    std::cout << "0. Back to main menu" << std::endl;
    std::cout << "Choose save to load: ";
}

void ConsoleRenderer::renderSaveMenu(const GameRenderData& data) {
    std::cout << "\n=== SAVE GAME ===" << std::endl;
    std::cout << "Enter save name (or 'cancel' to return): ";
}