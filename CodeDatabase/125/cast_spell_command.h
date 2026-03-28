#pragma once
#include "command.h"

class CastSpellCommand : public Command {
public:
    CastSpellCommand(int spellIndex);
    void Execute(GameSession& game) override;
    std::string GetDescription() const override;
    
private:
    int spellIndex_;
};
