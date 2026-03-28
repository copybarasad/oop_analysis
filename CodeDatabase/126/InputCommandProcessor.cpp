#include "../../headers/commands/InputCommandProcessor.h"
#include <iostream>
#include <string>

using namespace std;

unique_ptr<Command> InputCommandProcessor::getCommand() {
    char input;
    cout << "Enter command (WASD-move, X-switch weapon, R-ranged attack, C-cast spell, B-buy spell, O-save, L-load, Q-quit): ";
    cin >> input;

    switch(input) {
        case 'w': case 'W': return make_unique<MoveCommand>(Direction::UP);
        case 's': case 'S': return make_unique<MoveCommand>(Direction::DOWN);
        case 'a': case 'A': return make_unique<MoveCommand>(Direction::LEFT);
        case 'd': case 'D': return make_unique<MoveCommand>(Direction::RIGHT);
        case 'x': case 'X': return make_unique<SwitchWeaponCommand>();
        case 'r': case 'R': {
            Direction dir = getDirection();
            return make_unique<RangedAttackCommand>(dir);
        }
        case 'c': case 'C': return make_unique<CastSpellCommand>();
        case 'b': case 'B': return make_unique<BuySpellCommand>();
        case 'o': case 'O': return make_unique<SaveGameCommand>();
        case 'l': case 'L': return make_unique<LoadGameCommand>();
        case 'q': case 'Q': return make_unique<QuitCommand>();
        case 'n': case 'N': return make_unique<NextLevelCommand>();
        default:
            cout << "Invalid command!\n";
            return nullptr;
    }
}

Direction InputCommandProcessor::getDirection() {
    char direction;
    cout << "Enter direction (w-up, s-down, a-left, d-right): ";
    cin >> direction;

    switch(direction){
        case 'w': case 'W': return Direction::UP;
        case 's': case 'S': return Direction::DOWN;
        case 'a': case 'A': return Direction::LEFT;
        case 'd': case 'D': return Direction::RIGHT;
        default: 
            cout << "Invalid direction!\n"; 
            return Direction::UP;
    }
}

int InputCommandProcessor::getSpellIndex(int maxSpells) {
    int spellIndex = -1;
    bool validInput = false;
    while (!validInput) {
        cout << "Choose spell (0-" << maxSpells - 1 << "): ";
        string input;
        cin >> input;
        
        try {
            spellIndex = stoi(input);
            validInput = true;
        } catch (const invalid_argument&) {
            cout << "Invalid input! Please enter a number.\n";
            continue;
        } catch (const out_of_range&) {
            cout << "Number is too large! Please enter a smaller number.\n";
            continue;
        }
        
        if (spellIndex < 0 || spellIndex >= maxSpells) {
            cout << "Invalid spell index! Please enter between 0 and " << maxSpells - 1 << ".\n";
            validInput = false;
        }
    }
    return spellIndex;
}