#include "console_input_reader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include <unordered_set>
#include <unordered_map>
#include <vector>


static std::string normalizeCmd(const std::string &s) {
    std::string out;
    out.reserve(s.size());
    for (char ch : s) {
        if (ch == ' ' || ch == '_' ) continue;
        out.push_back(static_cast<char>(std::tolower((unsigned char)ch)));
    }
    return out;
}

void ConsoleInputReader::loadDefaultControls() {
    keymap.clear();

    keymap['W'] = CommandType::MoveUp;
    keymap['S'] = CommandType::MoveDown;
    keymap['A'] = CommandType::MoveLeft;
    keymap['D'] = CommandType::MoveRight;
    keymap['F'] = CommandType::Attack;
    keymap['R'] = CommandType::SwitchCombatMode;
    keymap['1'] = CommandType::CastSpell1;
    keymap['2'] = CommandType::CastSpell2;
    keymap['3'] = CommandType::CastSpell3;
    keymap['M'] = CommandType::SaveGame;
    keymap['L'] = CommandType::LoadGame;
    keymap['P'] = CommandType::Pause;
    keymap['Q'] = CommandType::Quit;

    std::cout << "[Controls] Using default controls.\n";
}

void ConsoleInputReader::loadFromFile() {
    std::ifstream file("controls.cfg");
    if (!file.is_open()) {
        std::cout << "[Controls] File not found.\n";
        loadDefaultControls();
        return;
    }

    std::unordered_map<char, CommandType> tempMap;
    std::unordered_set<CommandType> usedCommands;
    bool anyLine = false;
    std::string line;
    int lineNo = 0;

    static const std::unordered_map<std::string, CommandType> strToCmd = {
        {"moveup", CommandType::MoveUp},
        {"movedown", CommandType::MoveDown},
        {"moveleft", CommandType::MoveLeft},
        {"moveright", CommandType::MoveRight},
        {"attack", CommandType::Attack},
        {"switchcombatmode", CommandType::SwitchCombatMode},
        {"castspell1", CommandType::CastSpell1},
        {"castspell2", CommandType::CastSpell2},
        {"castspell3", CommandType::CastSpell3},
        {"savegame", CommandType::SaveGame},
        {"loadgame", CommandType::LoadGame},
        {"pause", CommandType::Pause},
        {"quit", CommandType::Quit}
    };

    while (std::getline(file, line)) {
        ++lineNo;
        size_t pos = 0;
        while (pos < line.size() && isspace((unsigned char)line[pos])) ++pos;
        if (pos >= line.size()) continue;

        if (line[pos] == '#') continue;

        std::istringstream iss(line);
        std::string keyToken, cmdToken;
        if (!(iss >> keyToken >> cmdToken)) {
            std::cout << "[Controls] Malformed line " << lineNo << ": \"" << line << "\"\n";
            loadDefaultControls();
            return;
        }
        anyLine = true;

        if (keyToken.size() != 1) {
            std::cout << "[Controls] Invalid key token at line " << lineNo << ": \"" << keyToken << "\"\n";
            loadDefaultControls();
            return;
        }

        char rawKey = keyToken[0];
        char key = static_cast<char>(std::toupper((unsigned char)rawKey));

        std::string norm = normalizeCmd(cmdToken);

        auto it = strToCmd.find(norm);
        if (it == strToCmd.end()) {
            std::cout << "[Controls] Unknown command at line " << lineNo << ": \"" << cmdToken << "\"\n";
            loadDefaultControls();
            return;
        }
        CommandType ct = it->second;

        if (tempMap.count(key)) {
            std::cout << "[Controls] Duplicate key at line " << lineNo << ": '" << key << "'\n";
            loadDefaultControls();
            return;
        }

        if (usedCommands.count(ct)) {
            std::cout << "[Controls] Duplicate command at line " << lineNo << ": \"" << cmdToken << "\"\n";
            loadDefaultControls();
            return;
        }

        tempMap[key] = ct;
        usedCommands.insert(ct);
    }

    if (!anyLine) {
        std::cout << "[Controls] File is empty.\n";
        loadDefaultControls();
        return;
    }


const std::vector<CommandType> required = {
    CommandType::MoveUp,
    CommandType::MoveDown,
    CommandType::MoveLeft,
    CommandType::MoveRight,
    CommandType::Attack,
    CommandType::SwitchCombatMode,
    CommandType::CastSpell1,
    CommandType::CastSpell2,
    CommandType::CastSpell3,
    CommandType::SaveGame,
    CommandType::LoadGame,
    CommandType::Pause,
    CommandType::Quit
};

for (CommandType rc : required) {
    if (!usedCommands.count(rc)) {
        std::cout << "[Controls] Missing command in file: required command not mapped.\n";
        loadDefaultControls();
        return;
    }
}

    keymap = std::move(tempMap);
    std::cout << "[Controls] Loaded custom controls.\n";
}

ConsoleInputReader::ConsoleInputReader() {
    loadFromFile();
}

Command ConsoleInputReader::readCommand() {
    std::string line;
    bool gotValidLine = false;
    while (!gotValidLine) {
        if (!std::getline(std::cin, line)) {
            return Command{CommandType::None};
        }

        size_t start = 0;
        while (start < line.size() && std::isspace((unsigned char)line[start])) ++start;
        size_t end = line.size();
        while (end > start && std::isspace((unsigned char)line[end - 1])) --end;

        if (start < end) {
            line = line.substr(start, end - start);
            gotValidLine = true;
        }
    }

    {
        std::istringstream iss(line);
        int s, x, y;
        if (iss >> s >> x >> y) {
            if (s >= 1 && s <= 3) {
                Command cmd;
                cmd.type = (s == 1) ? CommandType::CastSpell1 :
                           (s == 2) ? CommandType::CastSpell2 :
                                      CommandType::CastSpell3;
                cmd.paramX = x;
                cmd.paramY = y;
                return cmd;
            }
        }
    }

    std::string norm;
    norm.reserve(line.size());
    for (char ch : line) {
        if (ch != ' ' && ch != '_')
            norm.push_back(std::tolower((unsigned char)ch));
    }

    if (norm.size() == 1) {
        char c = std::toupper((unsigned char)norm[0]);

        auto it = keymap.find(c);
        if (it != keymap.end()) {
            return Command{it->second};
        }

        return Command{CommandType::None};
    }

    static const std::unordered_map<std::string, CommandType> words = {
        {"moveup", CommandType::MoveUp},
        {"movedown", CommandType::MoveDown},
        {"moveleft", CommandType::MoveLeft},
        {"moveright", CommandType::MoveRight},
        {"attack", CommandType::Attack},
        {"switchcombatmode", CommandType::SwitchCombatMode},
        {"castspell1", CommandType::CastSpell1},
        {"castspell2", CommandType::CastSpell2},
        {"castspell3", CommandType::CastSpell3},
        {"save", CommandType::SaveGame},
        {"savegame", CommandType::SaveGame},
        {"load", CommandType::LoadGame},
        {"loadgame", CommandType::LoadGame},
        {"pause", CommandType::Pause},
        {"quit", CommandType::Quit}
    };

    auto itw = words.find(norm);
    if (itw != words.end()) {
        return Command{itw->second};
    }

    return Command{CommandType::None};
}
