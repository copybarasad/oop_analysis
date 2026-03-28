#include "console_input_reader.h"

ConsoleInputReader::ConsoleInputReader()
    : max_spell_count(0), bindings()
{
    if (!bindings.load_from_file(KeyBindings::DEFAULT_CONFIG_PATH))
    {
        bindings.reset_to_default();
    }
}

ConsoleInputReader::ConsoleInputReader(const std::string& config_path)
    : max_spell_count(0), bindings()
{
    if (!bindings.load_from_file(config_path))
    {
        bindings.reset_to_default();
    }
}

void ConsoleInputReader::set_max_spell_count(int count)
{
    max_spell_count = count;
}

bool ConsoleInputReader::load_bindings(const std::string& filepath)
{
    return bindings.load_from_file(filepath);
}

std::string ConsoleInputReader::get_controls_help() const
{
    return bindings.get_controls_help();
}

std::unique_ptr<ICommand> ConsoleInputReader::create_move_command(char key) const
{
    int dx = bindings.get_move_dx(key);
    int dy = bindings.get_move_dy(key);
    return std::make_unique<MoveCommand>(dx, dy);
}

std::unique_ptr<ICommand> ConsoleInputReader::create_spell_command(int spell_index) const
{
    int x, y;
    std::cout << "\nEnter coordinates (x, y): ";
    
    if (!(std::cin >> x >> y))
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return std::make_unique<InvalidCommand>("Invalid coordinates input");
    }
    std::cin.ignore();
    
    return std::make_unique<CastSpellCommand>(spell_index, x, y);
}

std::unique_ptr<ICommand> ConsoleInputReader::read_command()
{
    std::string input;
    std::getline(std::cin, input);
    
    if (input.empty())
    {
        return std::make_unique<InvalidCommand>("Empty input");
    }
    
    char key = input[0];
    
    if (input == "space")
    {
        key = ' ';
    }
    else
    {
        key = std::tolower(key);
    }
    
    int spell_index = key - '0';
    if (spell_index > 0 && spell_index <= max_spell_count)
    {
        return create_spell_command(spell_index - 1);
    }
    
    CommandType cmd_type = bindings.get_command_type(key);
    
    switch (cmd_type)
    {
        case CommandType::MOVE:
            return create_move_command(key);
        case CommandType::ATTACK:
            return std::make_unique<AttackCommand>();
        case CommandType::SWITCH_MODE:
            return std::make_unique<SwitchModeCommand>();
        case CommandType::SAVE:
            return std::make_unique<SaveCommand>();
        case CommandType::LOAD:
            return std::make_unique<LoadCommand>();
        case CommandType::EXIT:
            return std::make_unique<ExitCommand>();
        case CommandType::INVALID:
        default:
            return std::make_unique<InvalidCommand>("Unknown key");
    }
}