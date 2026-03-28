#ifndef OOP_WASD_CMD_H
#define OOP_WASD_CMD_H
#include "base_command.h"


class WasdCommand : public Command{
    char symbol;
public:
    explicit WasdCommand(char symbol_);
    std::string get_name() override;
    char get_symbol() const;

};

#endif //OOP_WASD_CMD_H