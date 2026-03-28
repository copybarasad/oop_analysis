#include "ExtendedGame.h"

using namespace std;

void ExtendedGame::ProcessGameCommand(const GameCommand& command) {
    char input = ConvertCommandToChar(command);
    ProcessPlayerInput(input);
}

void ExtendedGame::ProcessPlayerInputPublic(char input) {
    ProcessPlayerInput(input);
}

bool ExtendedGame::IsGameRunning() const {
    return GetPlayer().IsAlive_player() && GetEnemy().IsAlive_enemy();
}

void ExtendedGame::Initialize() {
    isRunning = true;
    Renderer::RenderMainMenu();
    
    int choice;
    cin >> choice;
    
    GameSaveManager saveManager(this);
    
    if (choice == 2) {
        if (saveManager.ShowLoadMenuAtStart()) {
        } else {
            currentLevel = 1;
            StartLevel(currentLevel);
        }
    } else {
        currentLevel = 1;
        StartLevel(currentLevel);
    }
}

char ExtendedGame::ConvertCommandToChar(const GameCommand& command) {
    switch(command.type) {
        case GameCommand::Type::MOVE_UP: return 'w';
        case GameCommand::Type::MOVE_DOWN: return 's';
        case GameCommand::Type::MOVE_LEFT: return 'a';
        case GameCommand::Type::MOVE_RIGHT: return 'd';
        case GameCommand::Type::SHOW_STATS: return 'z';
        case GameCommand::Type::SHOW_INSTRUCTIONS: return 'i';
        case GameCommand::Type::SHOW_SPELLS: return 'h';
        case GameCommand::Type::BUY_SPELLS: return 'p';
        case GameCommand::Type::CAST_SPELL: return 'c';
        case GameCommand::Type::CHANGE_COMBAT_RANGE: return 'b';
        case GameCommand::Type::SAVE_GAME: return 'v';
        case GameCommand::Type::LOAD_GAME: return 'l';
        case GameCommand::Type::EXIT: return 'e';
        default: return ' ';
    }
}