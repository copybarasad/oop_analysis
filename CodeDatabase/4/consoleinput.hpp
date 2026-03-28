#pragma once
#include "inputreader.hpp"

class ConsoleInput : public InputReader {
public:
    Command read_command() override;
};