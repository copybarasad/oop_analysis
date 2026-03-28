#ifndef OOP_SAVE_CMD_H
#define OOP_SAVE_CMD_H

#include "base_command.h"

class SaveCommand : public Command{
public:
    std::string get_name() override;
};

#endif //OOP_SAVE_CMD_H