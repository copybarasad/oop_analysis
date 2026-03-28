#pragma once
#include "IRender.h"
#include "CommandBindings.h"
#include "Command.h"

class ConsoleRenderer : public IRender {
public:
    void SetBindings(const CommandBindings& b) {
        bindings_ = &b;
    }
    void RenderField(const GameField& field) override;
    void RenderHUD(const Game& game) override;
    void RenderMessage(const char* msg) override;
private:
    const CommandBindings* bindings_ = nullptr;
};