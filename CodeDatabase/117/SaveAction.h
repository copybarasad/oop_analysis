#ifndef SAVEACTION_H
#define SAVEACTION_H

#include "Action.h"
#include "../Exceptions/SaveLoadException.h"
#include "../Output/ConsoleRenderer.h"
#include "../Output/GameVisualizer.h"
#include "../Controllers/LevelSystem.h"

template <class InputHandler>
class SaveAction : public Action {
private:
    GameVisualizer<ConsoleRenderer>& view;
    InputHandler& handler;
    GameState& gameState;
    LevelSystem& levels;
public:
    SaveAction(InputHandler& handler, GameVisualizer<ConsoleRenderer>& view, GameState& gameState, LevelSystem& levels) :
        view(view), handler(handler), gameState(gameState), levels(levels) {}

    bool execute() override{
        this->view.renderMessage("Choose save slot (Enter 1 - " + std::to_string(MAX_SLOT_NUM) + ")");

        int number = this->handler.getNum();

        try {
            this->levels.saveToSlot(this->gameState, number);
        } catch (SaveLoadException& e) {
            this->view.renderErrorMessage(e.what());
        }
        return false;
    }

    std::string getName() const override{
        return "Save game";
    }
};

#endif