#include "keyboard_input.h"


GameCommand KeyboardInput::get_next_command() {
    std::cout << "\nEnter command: ";
    std::string line;
    if (!std::getline(std::cin, line)) {
        return GameCommand{CommandType::Quit};
    }

    std::istringstream iss(line);
    std::string key;
    iss >> key;

    GameCommand gc;

    auto it = bindings.keyToCommand.find(key);
    if (it == bindings.keyToCommand.end()) {
        gc.type = CommandType::None;
        return gc;
    }

    LogicalCommand lc = it->second;
    switch (lc) {
    case LogicalCommand::Quit:
        gc.type = CommandType::Quit;
        break;

    case LogicalCommand::Help:
        gc.type = CommandType::Help;
        break;

    case LogicalCommand::MoveUp:
    case LogicalCommand::MoveDown:
    case LogicalCommand::MoveLeft:
    case LogicalCommand::MoveRight:
        gc.type = CommandType::Move;
        if (lc == LogicalCommand::MoveUp)    gc.direction = "w";
        if (lc == LogicalCommand::MoveDown)  gc.direction = "s";
        if (lc == LogicalCommand::MoveLeft)  gc.direction = "a";
        if (lc == LogicalCommand::MoveRight) gc.direction = "d";
        break;

    case LogicalCommand::SwitchMode:
        gc.type = CommandType::SwitchMode;
        break;

    case LogicalCommand::Attack: {
        int x, y;
        if (iss >> x >> y) {
            gc.type  = CommandType::Attack;
            gc.x = x;
            gc.y = y;
        } else {
            gc.type = CommandType::None;
        }
        break;
    }

    case LogicalCommand::UseSpell: {
        std::size_t idx;
        int x, y;
        if (iss >> idx >> x >> y) {
            gc.type = CommandType::UseSpell;
            gc.spellIndex = idx;
            gc.x = x;
            gc.y = y;
        } else {
            gc.type = CommandType::None;
        }
        break;
    }

    case LogicalCommand::Save:
        gc.type = CommandType::Save;
        break;

    case LogicalCommand::Load:
        gc.type = CommandType::Load;
        break;

    default:
        gc.type = CommandType::None;
        break;
    }

    return gc;
}

void KeyboardInput::add_binding(const std::string& key, LogicalCommand cmd){
    bindings.keyToCommand[key] = cmd;
    bindings.commandToKey[cmd] = key;
}

void KeyboardInput::set_default() {
    bindings.keyToCommand.clear();
    bindings.commandToKey.clear();

    add_binding("q",    LogicalCommand::Quit);
    add_binding("h",    LogicalCommand::Help);
    add_binding("w",    LogicalCommand::MoveUp);
    add_binding("a",    LogicalCommand::MoveLeft);
    add_binding("s",    LogicalCommand::MoveDown);
    add_binding("d",    LogicalCommand::MoveRight);
    add_binding("m",    LogicalCommand::SwitchMode);
    add_binding("f",    LogicalCommand::Attack);
    add_binding("u",    LogicalCommand::UseSpell);
    add_binding("save", LogicalCommand::Save);
    add_binding("load", LogicalCommand::Load);
}

LogicalCommand KeyboardInput::parse_command(const std::string& s) {
    if (s == "quit")        return LogicalCommand::Quit;
    if (s == "help")        return LogicalCommand::Help;
    if (s == "move_up")     return LogicalCommand::MoveUp;
    if (s == "move_down")   return LogicalCommand::MoveDown;
    if (s == "move_left")   return LogicalCommand::MoveLeft;
    if (s == "move_right")  return LogicalCommand::MoveRight;
    if (s == "switch_mode") return LogicalCommand::SwitchMode;
    if (s == "attack")      return LogicalCommand::Attack;
    if (s == "use_spell")   return LogicalCommand::UseSpell;
    if (s == "save")        return LogicalCommand::Save;
    if (s == "load")        return LogicalCommand::Load;
    return LogicalCommand::Invalid;
}

bool KeyboardInput::load_from_file(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;

    KeyBindings temp;
    std::string line;

    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string key, eq, cmdName;
        if (!(iss >> key >> eq >> cmdName)) {
            return false;
        }
        if (eq != "=") {
            return false;
        }

        LogicalCommand cmd = parse_command(cmdName);
        if (cmd == LogicalCommand::Invalid) {
            return false;
        }

        // Конфликты:
        if (temp.keyToCommand.count(key))  return false;
        if (temp.commandToKey.count(cmd))  return false; 

        temp.keyToCommand[key] = cmd;
        temp.commandToKey[cmd] = key;
    }

    std::vector<LogicalCommand> required = {
        LogicalCommand::Quit,
        LogicalCommand::Help,
        LogicalCommand::MoveUp,
        LogicalCommand::MoveDown,
        LogicalCommand::MoveLeft,
        LogicalCommand::MoveRight,
        LogicalCommand::SwitchMode,
        LogicalCommand::Attack,
        LogicalCommand::UseSpell,
        LogicalCommand::Save,
        LogicalCommand::Load
    };

    for (auto c : required) {
        if (!temp.commandToKey.count(c)) {
            return false;
        }
    }

    bindings = std::move(temp);
    return true;
}