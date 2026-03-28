#include "Game.h"

#include "LogSystem/EndLevelEvent.h"
#include "LogSystem/Notifier.h"

Game::Game():
    gameState(GameState(Field(), Hand(), Player())),
    behaviour(BehaviourSystem(gameState)),
    levels(),
    movSys(),
    view(gameState),
    input(this->gameState, this->levels, view)
    {}

void Game::run(){
    if (!this->input.start()){
        return;
    }

    bool inCycleFlag = true;
    while(inCycleFlag){
        while(!this->gameState.isGameOver() && !this->gameState.isWin()){
            this->view.render();

            bool inputFlag = true;
            while(inputFlag){
                inputFlag = !this->input.processInput();
            }

            this->gameState.deleteDead();
            this->behaviour.alliesTurn();
            this->behaviour.enemiesTurn();
            this->behaviour.buildingsTurn();
            this->behaviour.updateTraps();
        }

        if(this->gameState.isWin()){
            EndLevelEvent eventWin = EndLevelEvent(this->levels.getLevelName(), true, this->gameState.getPlayer().getEXP());
            Notifier::getInstance().notify(eventWin);
            if(!this->input.goNext()){
                inCycleFlag = false;
            };
        }

        if(this->gameState.isGameOver()){
            EndLevelEvent eventLose = EndLevelEvent(this->levels.getLevelName(), false, this->gameState.getPlayer().getEXP());
            Notifier::getInstance().notify(eventLose);
            if(!this->input.restart()){
                inCycleFlag = false;
            }
        }
    }
}