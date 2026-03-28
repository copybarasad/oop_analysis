#pragma once

#include "Types.h"
#include "CommandMapper.h"
#include "Input.h"

class KeyboardInputReader : public IInputReader {
private:
    CommandMapper mapper;

public:
    Command poll() override {
        char input = getInput();
        return mapper.mapKey(input);
    }
};