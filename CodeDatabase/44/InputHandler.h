#pragma once
#include <memory>
#include <string>
#include <optional>
#include "Command.h"
#include "KeyBindings.h"

class InputHandler {
public:
    InputHandler();

    void LoadKeyBindings(const std::string& filename);
    void SetKeyBindings(const KeyBindings& bindings);

    std::unique_ptr<Command> ReadGameCommand();
    std::unique_ptr<MenuCommand> ReadMenuChoice();
    std::unique_ptr<Command> ReadYesNo();
    std::unique_ptr<TextInputCommand> ReadFileName();
    std::optional<int> ReadSpellIndex(int max_index);
    std::optional<int> ReadSaveSlot(int save_count);

private:
    bool ReadInt(int& value);
    bool ReadChar(char& c);
    std::unique_ptr<Command> CharToCommand(char c);

    KeyBindings key_bindings_;
};
