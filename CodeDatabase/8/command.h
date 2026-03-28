#pragma once
#include <string>
#include <sstream>
using namespace std;

template<typename RendererType>
class GameVisualization;
class Game;
class InputReader;

class Command {
public:
    virtual ~Command() {}
    virtual bool execute(Game& game, string& resultMessage) = 0;
    template<typename RendererType>
    bool executeWithVisualization(Game& game, GameVisualization<RendererType>& visualization, string& resultMessage) {
        bool success = execute(game, resultMessage);
        if (!resultMessage.empty()) {
            visualization.renderMessage(resultMessage);
        }
        return success;
    }
    virtual string getType() const = 0;
};