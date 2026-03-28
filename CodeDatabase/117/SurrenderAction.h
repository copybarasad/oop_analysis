#ifndef SURRENDERACTION_H
#define SURRENDERACTION_H

#include "Action.h"
#include "../Output/ConsoleRenderer.h"
#include "../Output/GameVisualizer.h"

class SurrenderAction : public Action {
private:
    GameVisualizer<ConsoleRenderer>& view;
    Player& player;
public:
    SurrenderAction(GameVisualizer<ConsoleRenderer>& view, Player& player);
    bool execute() override;
    std::string getName() const override;
};

#endif