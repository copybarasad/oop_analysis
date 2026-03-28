#include "input_reader.h"
#include "concrete_command.h"
#include <sstream>

string InputReader::readLine() const {
    string line;
    getline(inputStream, line);
    return line;
}

vector<string> InputReader::tokenize(const string& input) const {
    vector<string> tokens;
    stringstream ss(input);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

string InputReader::readInputLine() {
    return readLine();
}

vector<string> InputReader::readTokens() {
    string input = readLine();
    return tokenize(input);
}

int InputReader::readInteger() {
    int value;
    inputStream >> value;
    inputStream.ignore();
    return value;
}

vector<int> InputReader::readNumbers(int count) {
    vector<int> numbers;
    for (int i = 0; i < count; i++) {
        int num;
        inputStream >> num;
        numbers.push_back(num);
    }
    inputStream.ignore();
    return numbers;
}

template<typename RendererType>
Command* InputReader::readCommand() {
    string input = readInputLine();
    
    if (input.empty()) {
        return nullptr;
    }

    string commandName;
    if (commandMapper) {
        commandName = commandMapper->mapInputToCommand(input);
    } else {
        commandName = input;
    }
    
    if (commandName.empty()) {
        return nullptr;
    }
    
    return createCommandFromTokens<RendererType>({commandName});
}

template<typename RendererType>
Command* InputReader::createCommandFromTokens(const vector<string>& tokens) {
    if (tokens.empty()) return nullptr;
    
    string commandName = tokens[0];
    
    if (commandName == "UP") return new MoveUpCommand();
    if (commandName == "DOWN") return new MoveDownCommand();
    if (commandName == "LEFT") return new MoveLeftCommand();
    if (commandName == "RIGHT") return new MoveRightCommand();
    if (commandName == "ATTACK") return new AttackCommand();
    if (commandName == "SHOW_SPELLS") return new ShowSpellsCommand();
    if (commandName == "BUY_SPELL") return new BuySpellCommand();
    if (commandName == "USE_SPELL") return new UseSpellCommand();
    if (commandName == "CHANGE_TYPE") return new ChangeCombatTypeCommand();
    if (commandName == "SHOW_STATUS") return new ShowStatusCommand();
    if (commandName == "SAVE") return new SaveCommand();
    if (commandName == "LOAD") return new LoadCommand();
    if (commandName == "QUIT" || commandName == "EXIT") return new QuitCommand();
    if (commandName == "SHOW_MENU") return new ShowMenuCommand();
    
    return nullptr;
}

template Command* InputReader::readCommand<ConsoleRenderer>();
template Command* InputReader::readCommand<ColorfulRenderer>();
template Command* InputReader::createCommandFromTokens<ConsoleRenderer>(const vector<string>&);
template Command* InputReader::createCommandFromTokens<ColorfulRenderer>(const vector<string>&);