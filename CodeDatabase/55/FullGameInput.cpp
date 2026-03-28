// FullGameInput.cpp
#include "FullGameInput.h"
#include <iostream>

std::optional<Command> FullGameInput::getNextCommand() {
    bool needNewCommand = true;
    
    while (needNewCommand) {
        std::optional<Command> cmdOpt = (mode_ == Mode::Menu) ? menu_.getNextCommand() : gameplay_.getNextCommand();

        if (!cmdOpt) { continue;}

        const Command& cmd = *cmdOpt;
        needNewCommand = false;

        switch (cmd.id) {
            case CommandID::StartNewGame:
            case CommandID::SelectLevel:
            case CommandID::LoadGame:
                mode_ = Mode::Gameplay;
                break;

            case CommandID::Quit:
            case CommandID::ChooseUpgrade:
                break;

            default:
                if (mode_ == Mode::Gameplay) {
                    break;
                } else {
                    needNewCommand = true;
                    continue;
                }
        }
        return cmdOpt;
    }
    return std::nullopt;
}

void FullGameInput::switchToGameplay() {
    mode_ = Mode::Gameplay;
}

void FullGameInput::switchToMenu() {
    mode_ = Mode::Menu;
    menu_.reset();
}