#include "wasd_cmd.h"

std::string WasdCommand::get_name() {
    return "WASD";
}

WasdCommand::WasdCommand(char symbol_) : symbol(symbol_) {}

char WasdCommand::get_symbol() const {
    return symbol;
}



