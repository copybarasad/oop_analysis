#ifndef OOP_LOAD_CMD_H
#define OOP_LOAD_CMD_H

#include "base_command.h"

class LoadCommand : public Command{
public:
    std::string get_name() override;
};

#endif //OOP_LOAD_CMD_H