#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

enum class CommandType {
    MoveUp,
    MoveDown, 
    MoveLeft,
    MoveRight,
    Attack,
    SwitchMode,
    CastSpell1,
    CastSpell2,
    CastSpell3,
    OpenShop,
    SaveGame,
    Control,
    Invalid
};

class InputConfig {
public:
    InputConfig();
    
    static InputConfig& GetInstance();
    static void SetInstance(const InputConfig& config);
    
    bool LoadFromFile(const std::string& filename);
    CommandType GetCommandType(char input) const;
    char GetInputSymbol(CommandType command) const;
    bool IsValid() const { return valid_; }
    std::string GetHelpString() const;
    
private:
    void SetDefaultConfig();
    bool ValidateConfig() const;

    std::unordered_map<char, CommandType> key_to_command_;
    std::unordered_map<CommandType, char> command_to_key_;
    bool valid_;
};
