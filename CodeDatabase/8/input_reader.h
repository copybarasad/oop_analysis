#pragma once
#include "command.h"
#include "command_mapper.h"
#include <iostream>
#include <vector>
using namespace std;

class Game;
class Command;

class InputReader {
private:
    istream& inputStream;
    CommandMapper* commandMapper;
    
public:
    InputReader(istream& stream = cin) 
        : inputStream(stream), commandMapper(nullptr) {}

    InputReader(istream& stream, CommandMapper& mapper) 
        : inputStream(stream), commandMapper(&mapper) {}
    
    template<typename RendererType>
    Command* readCommand();

    string readInputLine();
    vector<string> readTokens();
    int readInteger();
    vector<int> readNumbers(int count);

private:
    string readLine() const;
    vector<string> tokenize(const string& input) const;
    template<typename RendererType>
    Command* createCommandFromTokens(const vector<string>& tokens);
};