#ifndef OOP_E_CMD_H
#define OOP_E_CMD_H
#include "base_command.h"

class KickCommand : public Command{
public:
    std::string get_name() override;
};

#endif //OOP_E_CMD_H