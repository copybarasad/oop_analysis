#ifndef ATTACKACTION_H
#define ATTACKACTION_H

#include "Action.h"
#include "../Output/ConsoleRenderer.h"
#include "../Output/GameVisualizer.h"

template <class InputHandler>
class AttackAction : public Action {
private:
    InputHandler& handler;
    GameState& gameState;
    GameVisualizer<ConsoleRenderer>& view;
public:
    AttackAction(InputHandler& handler, GameVisualizer<ConsoleRenderer>& view, GameState& gameState) : 
        handler(handler), view(view), gameState(gameState) {}

    bool execute() override{
        this->view.renderMessage("Input coordinates");
        std::pair<int, int> coordinates = handler.getPosition();
        Character* target = this->gameState.findCharacter(coordinates);
        if (target != nullptr){
            if(gameState.getPlayer().attack(*target)){
                this->view.renderMessage("You made attack");
                return true;
            }
        }
        this->view.renderMessage("Couldn't attack");
        return false;
    }

    std::string getName() const override{
        return "Attack";
    }
};

#endif