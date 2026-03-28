#ifndef INPUT_HANDLER_H_
#define INPUT_HANDLER_H_

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>

class InputHandler {
public:
    struct Command {
        std::string action;
        std::vector<std::string> args;
        
        Command() : action("unknown") {}
        Command(const std::string& a, const std::vector<std::string>& ar = {}) 
            : action(a), args(ar) {}
        
        bool HasArgs(int count) const { return args.size() >= count; }
        int GetIntArg(int index) const {
            if (index < args.size()) {
                try {
                    return std::stoi(args[index]);
                } catch (...) {
                    return 0;
                }
            }
            return 0;
        }
        
        std::string GetStringArg(int index) const {
            if (index < args.size()) {
                return args[index];
            }
            return "";
        }
    };
    
    std::vector<std::string> SplitString(const std::string& str, char delimiter) const;
    Command ParseInput(const std::string& input) const;
    void LoadControls(); 
    std::map<std::string, std::string> GetDefaultControls() const;  
    bool ValidateControls(const std::map<std::string, std::string>& controls) const;  
    
    static bool IsMovementCommand(const std::string& input);
    static bool IsAttackCommand(const std::string& input);
    static bool IsBuildCommand(const std::string& input);
    static bool IsSpellCommand(const std::string& input);
    static bool IsModeSwitchCommand(const std::string& input);
    static bool IsHelpCommand(const std::string& input);
    static bool IsSaveCommand(const std::string& input);
    static bool IsLoadCommand(const std::string& input);
    static bool IsExitCommand(const std::string& input);

private:
    std::map<std::string, std::string> controls_; 
};

#endif // INPUT_HANDLER_H_
