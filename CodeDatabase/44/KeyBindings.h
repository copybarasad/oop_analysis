#pragma once
#include "Command.h"
#include <map>
#include <string>

class KeyBindings {
public:
    KeyBindings();

    void LoadFromFile(const std::string& filename);

    CommandType GetCommandType(char key) const;

    std::pair<int, int> GetMoveOffset(CommandType type) const;

    bool IsMoveCommand(CommandType type) const;

    char GetKeyForCommand(CommandType type) const;

private:
    void SetDefaults();
    bool ValidateBindings(const std::map<char, CommandType>& bindings) const;

    std::map<char, CommandType> key_to_command_;

    static constexpr char kDefaultMoveUp = 'w';
    static constexpr char kDefaultMoveDown = 's';
    static constexpr char kDefaultMoveLeft = 'a';
    static constexpr char kDefaultMoveRight = 'd';
    static constexpr char kDefaultAttack = 'f';
    static constexpr char kDefaultSwitchWeapon = 'q';
    static constexpr char kDefaultUseSpell = 'e';
    static constexpr char kDefaultViewSpells = 'v';
    static constexpr char kDefaultSaveGame = 'z';
    static constexpr char kDefaultLoadGame = 'x';
    static constexpr char kDefaultQuit = 'p';
};
