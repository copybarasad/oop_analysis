#ifndef OOP_QUIT_CMD_H
#define OOP_QUIT_CMD_H

#include "base_command.h"

class QuitCommand : public Command{
public:
    std::string get_name() override;
};

#endif //OOP_QUIT_CMD_H