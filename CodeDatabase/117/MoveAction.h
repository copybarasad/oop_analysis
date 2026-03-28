#ifndef MOVEACTION_H
#define MOVEACTION_H

#include "Action.h"
#include "../Output/ConsoleRenderer.h"
#include "../Output/GameVisualizer.h"

class MoveAction : public Action {
private:
    GameVisualizer<ConsoleRenderer>& view;
    int dx, dy;
    std::string direction;
    Field& field;
    Player& player;
public:
    MoveAction(GameVisualizer<ConsoleRenderer>& view, int dx, int dy, const std::string& dir, Field& field, Player& player);
    bool execute() override;
    std::string getName() const override;
};

#endif