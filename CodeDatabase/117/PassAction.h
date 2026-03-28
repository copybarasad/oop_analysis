#ifndef PASSACTION_H
#define PASSACTION_H

#include "Action.h"
#include "../Output/ConsoleRenderer.h"
#include "../Output/GameVisualizer.h"

class PassAction : public Action {
private:
    GameVisualizer<ConsoleRenderer>& view;
public:
    PassAction(GameVisualizer<ConsoleRenderer>& view);
    bool execute() override;
    std::string getName() const override;
};

#endif