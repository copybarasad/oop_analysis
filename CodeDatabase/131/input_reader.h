#ifndef OOP_INPUT_READER_H_
#define OOP_INPUT_READER_H_

#include "command.h"
#include "key_bindings_manager.h"
#include <memory>
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace rpg {
    class ConsoleInputReader final {
    public:
        ConsoleInputReader();
        explicit ConsoleInputReader(const std::string& config_file);
        
        [[nodiscard]] std::unique_ptr<ICommand> ReadCommand() const;
        
        void LoadKeyBindings(const std::string& config_file);
        
        [[nodiscard]] const KeyBindingsManager& GetKeyBindingsManager() const {
            return key_bindings_;
        }
        
    private:
        KeyBindingsManager key_bindings_;
        
        [[nodiscard]] char ReadKey() const;
        
        [[nodiscard]] std::unique_ptr<ICommand> CreateCommand(CommandType cmd_type) const;
    };
}

#endif

