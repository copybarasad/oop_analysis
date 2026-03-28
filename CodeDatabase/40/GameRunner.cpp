#include "GameRunner.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>

void GameRunner::showMenu() {
    std::cout << "\n=== GAME MENU ===\n";
    std::cout << "[N] New Game\n";
    std::cout << "[L] Load Game\n";
    std::cout << "[Q] Quit\n";
    std::cout << "Choose: ";
}

void GameRunner::startNewGame(){
    try{
        currentGame = Game();
        currentGame.setGameRunnerRef(this);

        gameRunning = true;
        std::cout << "New Game started.\n";
    }catch (const std::exception& enemy){
        std::cerr << "Error starting new game: " << enemy.what() << "\n";
        gameRunning = false;
    }
}

void GameRunner::saveGame(){
    const std::string filename = "savegame.dat";
    try{
        std::ofstream file(filename, std::ios::binary);
        if(!file.is_open()){
            throw std::runtime_error("Cannot create save file: " + filename);
        }
        currentGame.save(file);

        std::cout << "Game saved to " << filename << ".\n";
    }catch (const std::exception& e){
        std::cerr << "Error saving game: " << e.what() << "\n";
    }
}


void GameRunner::loadGame(){
    const std::string filename = "savegame.dat";
    try {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()){
            throw std::runtime_error("Cannot open save file: " + filename + " (does not exist or is locked)");
        }

        currentGame = Game();
        currentGame.setGameRunnerRef(this);
        currentGame.load(file);

        gameRunning = true;
        std::cout << "Game loaded from " << filename << ".\n";
    } catch (const std::exception& enemy){
        std::cerr << "Error loading game: " << enemy.what() << "\n";
        gameRunning = false;
    }
}

void GameRunner::run(){
    while (true){
        showMenu();
        char input;
        std::cin >> input;
        switch (std::toupper(input)){
            case 'N':
                startNewGame();
                break;
            case 'L':
                loadGame();
                break;
            case 'Q':
                std::cout << "Goodbye!\n";
                return;
            default:
                std::cout << "Invalid command";
                break;
        }

        if (gameRunning){
            currentGame.run();
            handleGameEnd();
        }
    }
}

void GameRunner::handleGameEnd(){
    if(currentGame.isPlayerAlive()){
        std::cout << "You win!\n";
    }else{
        std::cout << "You lose!\n";
    }
    gameRunning = false;
    std::cout << "Press [R] to restart, or [M] for main menu: ";
    char choice;
    std::cin >> choice;
    if (choice == 'R'){
        startNewGame();
    }
}