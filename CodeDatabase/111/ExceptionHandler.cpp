#include "ExceptionHandler.h"
#include <iostream>

void ExceptionHandler::handleSaveLoadError(const SaveLoadException& e) {
    std::cerr << "\n SAVE/LOAD ERROR: " << e.what() << "\n";
    std::cerr << "Please check file permissions or try a different filename.\n";
}

void ExceptionHandler::handleGameError(const GameException& e) {
    std::cerr << "\n GAME ERROR: " << e.what() << "\n";
    std::cerr << "The game may be in an unstable state.\n";
}

void ExceptionHandler::handleGenericError(const std::exception& e) {
    std::cerr << "\n UNEXPECTED ERROR: " << e.what() << "\n";
    std::cerr << "Please restart the game.\n";
}

bool ExceptionHandler::shouldRetryOperation(const std::string& operation) {
    std::cout << "Retry " << operation << "? (y/n): ";
    char response;
    std::cin >> response;
    return response == 'y' || response == 'Y';
}