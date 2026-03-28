#pragma once
#include "general_manager.h"

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void render(GeneralManager& manager) = 0;
    virtual void printStatus(GeneralManager& manager) = 0;
    virtual void printMessage(const std::string& message) = 0;
    virtual void printLvlUpMenu() = 0;
    virtual void printStartMenu() = 0;
    virtual void clearScreen() const = 0;
};