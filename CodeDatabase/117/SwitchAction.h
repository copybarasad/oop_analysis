#ifndef SWITCHACTION_H
#define SWITCHACTION_H

#include "Action.h"
#include "../Output/ConsoleRenderer.h"
#include "../Output/GameVisualizer.h"

class SwitchAction : public Action {
private:
    GameVisualizer<ConsoleRenderer>& view;
    Player& player;
public:
    SwitchAction(GameVisualizer<ConsoleRenderer>& view, Player& player);
    bool execute() override;
    std::string getName() const override;
};

#endif