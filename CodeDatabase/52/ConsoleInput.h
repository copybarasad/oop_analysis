#pragma once
#include "ICommandSource.h"
#include "CommandBindings.h"

class ConsoleInput : public ICommandSource {
public:
    ConsoleInput();

    Command GetCommand() override;

    const CommandBindings& Bindings() const noexcept { return bindings_; }

private:
    CommandBindings bindings_;
};
