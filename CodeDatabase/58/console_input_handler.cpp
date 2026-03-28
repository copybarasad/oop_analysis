#include "console_input_handler.hpp"
#include "i_renderer.hpp"

ConsoleInputHandler::ConsoleInputHandler() : keybindings_manager(new KeybindingsManager()){
    command_map = keybindings_manager->get_keybindings();
}

ConsoleInputHandler::~ConsoleInputHandler(){
    delete keybindings_manager;
}

CommandManager ConsoleInputHandler::parse_input(std::string input) {
    std::string upper_input = input;
    std::transform(upper_input.begin(), upper_input.end(), upper_input.begin(), ::toupper);

    if (upper_input.empty()) return CommandManager(CommandType::UNKNOWN);

    auto it = command_map.find(upper_input);
    if (it != command_map.end()){
        return CommandManager(it->second);
    }
    return CommandManager(CommandType::UNKNOWN);
}

std::string ConsoleInputHandler::read_input(){
    std::string input;
    std::cout << "your turn: ";
    std::getline(std::cin, input);
    return input;
}

std::string ConsoleInputHandler::get_input(std::string msg){
    std::string input;
    std::cout << msg;
    std::cin >> input;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

void ConsoleInputHandler::setup_keybindings_interactive(IRenderer* renderer){
    keybindings_manager->setup_keybindings_interactive(renderer);
    command_map = keybindings_manager->get_keybindings();
}