#ifndef OOP_KEY_BINDINGS_MANAGER_H_
#define OOP_KEY_BINDINGS_MANAGER_H_

#include "command_types.h"
#include <map>
#include <set>
#include <string>
#include <fstream>

namespace rpg {

    class KeyBindingsManager final {
    public:
        KeyBindingsManager();
        
        bool LoadFromFile(const std::string& filename);
        
        [[nodiscard]] CommandType GetCommandType(char key) const;
        
        [[nodiscard]] const std::map<char, CommandType>& GetBindings() const {
            return bindings_;
        }
        
        void SetDefaultBindings();
        
    private:
        std::map<char, CommandType> bindings_;
        std::set<CommandType> required_commands_;
        
        bool ValidateBindings() const;
        
        bool HasDuplicateCommands() const;
        
        bool HasAllRequiredCommands() const;
        
        [[nodiscard]] CommandType StringToCommandType(const std::string& str) const;
        
        void InitializeRequiredCommands();
    };
}

#endif

