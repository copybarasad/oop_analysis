#ifndef LOAD_SELECTED_SAVE_COMMAND_H
#define LOAD_SELECTED_SAVE_COMMAND_H

#include "command_interface.h"

class LoadSelectedSaveCommand : public ICommand {
private:
    int saveIndex;
    
public:
    LoadSelectedSaveCommand(int index);
    bool execute(GameController& game) override;
    std::string getDescription() const override;
    bool isValid() const override;
    CommandType getCommandType() const override { return CommandType::LoadSelectedSave; }
    int getCommandData() const override { return saveIndex; }
    int getSaveIndex() const { return saveIndex; }
};

#endif