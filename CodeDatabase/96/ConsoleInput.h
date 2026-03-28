#pragma once
#include "IInput.h"
#include <string>

class ConsoleInput : public IInput {
public:
    std::string Read() override;
    ~ConsoleInput() override = default;
};