#pragma once
#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#include "ICommand.h"
#include <memory>
#include <string>

class InputProcessor {
public:
    virtual ~InputProcessor() = default;

    virtual std::unique_ptr<ICommand> getNextCommand() = 0;
    virtual void showPrompt(const std::string& prompt) = 0;
    virtual void showMessage(const std::string& message) = 0;
};

#endif 