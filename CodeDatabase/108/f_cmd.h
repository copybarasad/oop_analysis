#ifndef OOP_F_CMD_H
#define OOP_F_CMD_H
#include "base_command.h"

class NewCombatModeCommand : public Command{
public:
    std::string get_name() override;
};

#endif //OOP_F_CMD_H