#include "ConsoleInputHandler.h"
#include <iostream>
#include <limits>

ConsoleInputHandler::ConsoleInputHandler(){
    this->config = InputConfig();
    this->config.setup();
}

ActionType ConsoleInputHandler::getCommandType() const{
    char key = getChar();
    ActionType command = this->config.getAction(key);
    return command;
}

int ConsoleInputHandler::getNum() const{
    int num = -1;
    std::cin >> num;
    if (std::cin.fail()){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    return num;
}   

bool ConsoleInputHandler::getYesOrNo() const{
    int inp = this->getNum();
    if (inp == 1){
        return true;
    }
    return false;
}

char ConsoleInputHandler::getChar() const{
    std::string inp;
    std::cin >> inp;
    if (std::cin.fail() || inp.length() != 1) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return 0;
    }
    return std::toupper(inp[0]);
}

std::pair<int, int> ConsoleInputHandler::getPosition() const{
    int first = -1;
    int second = -1;
    std::cin >> first >> second;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return {-1, -1};
    }
    return {first, second};
}

MainMenuChoice ConsoleInputHandler::getMainMenuChoice() const{
    int choice = this->getNum();
    if (choice > 3 || choice < 1){
        throw std::runtime_error("Wrong input: Invalid choice");
    }
    return (MainMenuChoice)choice;
}

UpgradeType ConsoleInputHandler::getUpgradeType() const{
    int choice = this->getNum();
    if (choice > 4 || choice < 1){
        throw std::runtime_error("Wrong input: Invalid choice");
    }
    return (UpgradeType)choice;
}

std::map<char, ActionType> ConsoleInputHandler::getCharToCommand() const{
    return this->config.getCharToCommand();
}