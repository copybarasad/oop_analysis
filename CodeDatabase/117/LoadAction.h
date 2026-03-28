#ifndef LOADACTION_H
#define LOADACTION_H

#include "Action.h"
#include "../Exceptions/SaveLoadException.h"
#include "../Output/ConsoleRenderer.h"
#include "../Output/GameVisualizer.h"
#include "../Controllers/LevelSystem.h"

template <class InputHandler>
class LoadAction : public Action {
private:
    InputHandler& handler;
    GameVisualizer<ConsoleRenderer>& view;
    GameState& gameState;
    LevelSystem& levels;
public:
    LoadAction(InputHandler& handler, GameVisualizer<ConsoleRenderer>& view, GameState& gameState, LevelSystem& levelSystem) : 
        handler(handler), view(view), gameState(gameState), levels(levelSystem) {}

    bool execute() override{
        this->view.renderMessage("Choose save slot (Enter 1 - " + std::to_string(MAX_SLOT_NUM) + ")");

        int number = this->handler.getNum();

        try {
            this->levels.loadSave(this->gameState, number);
            this->view.renderMessage("Level - " + this->levels.getLevelName());
        }
        catch (SaveLoadException& e) {
            this->view.renderErrorMessage(e.what());
        }
        return false;
    }

    std::string getName() const override{
        return "Load game";
    }
};

#endif