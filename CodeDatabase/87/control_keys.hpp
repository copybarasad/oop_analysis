#ifndef CONTROL_KEYS_HPP
#define CONTROL_KEYS_HPP
#include <unordered_map>
#include <string>
#include <vector>

class Control_keys{
    public:
        enum class Command_num{
            MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, SWITCH_MELEE, SWITCH_RANGED, RANGED_ATTACK, SHOW_HELP, BUY_SPELL,
            QUIT, SAVE, LOAD, SPELL_CAST_1, SPELL_CAST_2, SPELL_CAST_3, COUNT
        };
        Control_keys();
        void save_to_file(const std::string& filename);
        char get_key(Command_num cmd);
        Command_num get_command(char key);
    private:
        std::unordered_map<Command_num, char> command_to_key;
        std::unordered_map<char, Command_num> key_to_command;
        bool is_valid = true;
        void load_default();
        void load_from_file(const std::string& filename);
        bool validate();
};
#endif