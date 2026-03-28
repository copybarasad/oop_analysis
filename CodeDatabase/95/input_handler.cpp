#include "input_handler.hpp"
#include <cctype>
#include <iostream>
#include <termios.h>
#include <unistd.h>

char getch() {
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char c;
    read(STDIN_FILENO, &c, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}

std::unique_ptr<Command> TerminalInputHandler::getNextCommand(GameMode mode) {
    switch (mode) {
        case GameMode::MAIN_MENU:
            return readMainMenuCommand();
        case GameMode::PAUSE_MENU:
            return readPauseMenuCommand();
        case GameMode::SAVE_MENU:
            return readSaveMenuCommand();
        case GameMode::IN_GAME:
            return readGameCommand();
        default:
            return std::make_unique<MenuCommand>();
    }
}

std::unique_ptr<MenuCommand> TerminalInputHandler::readMainMenuCommand() {
    char c;
    std::cin >> c;
    
    c = std::tolower(c);
    
    switch (c) {
        case '1':
        case 'n':
            return std::make_unique<MenuCommand>(MenuAction::START_NEW_GAME);
        case '2':
        case 'l':
            return std::make_unique<MenuCommand>(MenuAction::LOAD_GAME);
        case '3':
        case 'q':
            return std::make_unique<MenuCommand>(MenuAction::EXIT);
        default:
            return std::make_unique<MenuCommand>(MenuAction::NONE);
    }
}

std::unique_ptr<MenuCommand> TerminalInputHandler::readPauseMenuCommand() {
    char c;
    std::cin >> c;
    
    c = std::tolower(c);
    
    switch (c) {
        case '1':
            return std::make_unique<MenuCommand>(MenuAction::START_NEW_GAME);
        case '2':
            return std::make_unique<MenuCommand>(MenuAction::LOAD_GAME);
        case '3':
            return std::make_unique<MenuCommand>(MenuAction::SAVE_GAME_NAME);
        case '4':
            return std::make_unique<MenuCommand>(MenuAction::BACK);
        case '5':
        case 'q':
            return std::make_unique<MenuCommand>(MenuAction::EXIT);
        default:
            return std::make_unique<MenuCommand>(MenuAction::NONE);
    }
}

std::unique_ptr<MenuCommand> TerminalInputHandler::readSaveMenuCommand() {
    char c;
    std::cin >> c;
    
    if (c >= '1' && c <= '9') {
        int index = c - '0';
        return std::make_unique<MenuCommand>(MenuAction::SELECT_SAVE, index);
    }
    
    c = std::tolower(c);
    
    switch (c) {
        case 'b':
        case '0':
            return std::make_unique<MenuCommand>(MenuAction::BACK);
        case 'q':
            return std::make_unique<MenuCommand>(MenuAction::EXIT);
        default:
            return std::make_unique<MenuCommand>(MenuAction::NONE);
    }
}

std::unique_ptr<GameCommand> TerminalInputHandler::readGameCommand() {
    char c = getch();
    
    if (c == 27) {
        return std::make_unique<GameCommand>(GameCommand::Pause());
    }
    
    c = std::tolower(c);
    
    switch (c) {
        case 'w': return std::make_unique<GameCommand>(GameCommand::Move(MoveDir::UP));
        case 's': return std::make_unique<GameCommand>(GameCommand::Move(MoveDir::DOWN));
        case 'a': return std::make_unique<GameCommand>(GameCommand::Move(MoveDir::LEFT));
        case 'd': return std::make_unique<GameCommand>(GameCommand::Move(MoveDir::RIGHT));
        case 'p': return std::make_unique<GameCommand>(GameCommand::Pause());
        case 'e': return std::make_unique<GameCommand>(GameCommand::CastSpell(0));
        default:
            return std::make_unique<GameCommand>();
    }
}