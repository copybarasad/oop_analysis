#ifndef INFOACTION_H
#define INFOACTION_H

#include "Action.h"
#include "../Output/ConsoleRenderer.h"
#include "../Output/GameVisualizer.h"

class InfoAction : public Action {
private:
    Hand& hand;
    Player& player;
    GameVisualizer<ConsoleRenderer>& view;

public:
    InfoAction(GameVisualizer<ConsoleRenderer>& view, Hand& hand, Player& player);
    bool execute() override;
    std::string getName() const override;
};

#endif