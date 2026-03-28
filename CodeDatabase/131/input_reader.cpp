#include "input_reader.h"
#include "game.h"
#include <iostream>

namespace rpg {
    ConsoleInputReader::ConsoleInputReader() {
    }

    ConsoleInputReader::ConsoleInputReader(const std::string& config_file) {
        LoadKeyBindings(config_file);
    }

    void ConsoleInputReader::LoadKeyBindings(const std::string& config_file) {
        key_bindings_.LoadFromFile(config_file);
    }

    char ConsoleInputReader::ReadKey() const {
#ifdef _WIN32
        return static_cast<char>(_getch());
#else
        struct termios oldt, newt;
        char ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = static_cast<char>(getchar());
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
#endif
    }

    std::unique_ptr<ICommand> ConsoleInputReader::ReadCommand() const {
        std::cout << "\nВведите команду: ";
        std::cout.flush();
        
        char key = ReadKey();
        std::cout << key << "\n";
        
        CommandType cmd_type = key_bindings_.GetCommandType(key);
        return CreateCommand(cmd_type);
    }

    std::unique_ptr<ICommand> ConsoleInputReader::CreateCommand(CommandType cmd_type) const {
        switch (cmd_type) {
            case CommandType::kMoveUp:
                return std::make_unique<MoveCommand>(Direction::kUp);
            case CommandType::kMoveDown:
                return std::make_unique<MoveCommand>(Direction::kDown);
            case CommandType::kMoveLeft:
                return std::make_unique<MoveCommand>(Direction::kLeft);
            case CommandType::kMoveRight:
                return std::make_unique<MoveCommand>(Direction::kRight);
            case CommandType::kAttack:
                return std::make_unique<AttackCommand>();
            case CommandType::kSwitchMode:
                return std::make_unique<SwitchCombatModeCommand>();
            case CommandType::kBuySpell:
                return std::make_unique<BuySpellCommand>();
            case CommandType::kCastSpell0:
                return std::make_unique<CastSpellCommand>(0);
            case CommandType::kCastSpell1:
                return std::make_unique<CastSpellCommand>(1);
            case CommandType::kCastSpell2:
                return std::make_unique<CastSpellCommand>(2);
            case CommandType::kCastSpell3:
                return std::make_unique<CastSpellCommand>(3);
            case CommandType::kCastSpell4:
                return std::make_unique<CastSpellCommand>(4);
            case CommandType::kSaveGame:
                return std::make_unique<SaveGameCommand>();
            case CommandType::kQuit:
                return std::make_unique<QuitCommand>();
            default:
                return std::make_unique<NullCommand>();
        }
    }
}
