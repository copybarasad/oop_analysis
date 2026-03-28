#pragma once
#include "Command.h"
#include <memory>
#include <optional>

template<typename TInputHandler>
class GameController {
public:
    GameController() : input_() {}

    std::unique_ptr<Command> GetGameCommand() {
        return input_.ReadGameCommand();
    }

    std::unique_ptr<MenuCommand> GetMenuChoice() {
        return input_.ReadMenuChoice();
    }

    std::unique_ptr<Command> GetYesNo() {
        return input_.ReadYesNo();
    }

    std::unique_ptr<TextInputCommand> GetFileName() {
        return input_.ReadFileName();
    }

    std::optional<int> GetSpellIndex(int max_index) {
        return input_.ReadSpellIndex(max_index);
    }

    std::optional<int> GetSaveSlot(int save_count) {
        return input_.ReadSaveSlot(save_count);
    }

    TInputHandler& GetInputHandler() { return input_; }
    const TInputHandler& GetInputHandler() const { return input_; }

private:
    TInputHandler input_;
};
