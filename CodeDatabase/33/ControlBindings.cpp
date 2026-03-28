#include "ControlBindings.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace {
    ControlBindings::Mapping MakeMove(int dx, int dy) {
        ControlBindings::Mapping m;
        m.type = CommandType::Move;
        m.dx = dx; m.dy = dy;
        return m;
    }

    ControlBindings::Mapping MakeAttack(int dx, int dy) {
        ControlBindings::Mapping m;
        m.type = CommandType::Attack;
        m.dx = dx; m.dy = dy;
        return m;
    }
}

ControlBindings ControlBindings::MakeDefault() {
    ControlBindings cb;
    cb.using_default_ = true;
    cb.bindings_ = {
        {'w', MakeMove(0, -1)},
        {'s', MakeMove(0, 1)},
        {'a', MakeMove(-1, 0)},
        {'d', MakeMove(1, 0)},

        {'i', MakeAttack(0, -1)},
        {'k', MakeAttack(0, 1)},
        {'j', MakeAttack(-1, 0)},
        {'l', MakeAttack(1, 0)},

        {'m', {CommandType::ToggleMode, 0, 0}},
        {'c', {CommandType::CastSpell, 0, 0}},
        {'q', {CommandType::Quit, 0, 0}}
    };
    return cb;
}

bool ControlBindings::IsValidSet(const std::map<char, Mapping>& candidate) {
    const std::set<CommandType> required = {
        CommandType::Move, CommandType::Attack,
        CommandType::ToggleMode, CommandType::CastSpell,
        CommandType::Quit
    };

    std::set<CommandType> seen_types;
    std::set<char> seen_keys;

    for (const auto& kv : candidate) {
        if (seen_keys.count(kv.first)) {
            return false;
        }
        seen_keys.insert(kv.first);
        if (kv.second.type == CommandType::Invalid) {
            return false;
        }
        if (seen_types.count(kv.second.type) && kv.second.type != CommandType::Move && kv.second.type != CommandType::Attack) {
            return false;
        }
        seen_types.insert(kv.second.type);
    }

    int move_count = 0;
    int attack_count = 0;
    for (const auto& kv : candidate) {
        if (kv.second.type == CommandType::Move) move_count++;
        if (kv.second.type == CommandType::Attack) attack_count++;
    }
    if (move_count != 4 || attack_count != 4) return false;

    for (auto req : required) {
        if (req == CommandType::Move || req == CommandType::Attack) continue;
        if (!seen_types.count(req)) return false;
    }
    return true;
}

ControlBindings ControlBindings::LoadOrDefault(const std::string& path) {
    std::ifstream fin(path);
    if (!fin) {
        return MakeDefault();
    }

    std::map<char, Mapping> parsed;
    std::set<char> used_keys;
    std::set<std::string> used_actions;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        auto pos = line.find('=');
        if (pos == std::string::npos || pos+1 >= line.size()) continue;
        std::string action = line.substr(0, pos);
        std::string key_str = line.substr(pos+1);
        if (key_str.size() != 1) continue;
        char key = static_cast<char>(std::tolower(static_cast<unsigned char>(key_str[0])));

        if (used_keys.count(key) || used_actions.count(action)) {
            return MakeDefault();
        }

        Mapping map;
        if (action == "MOVE_UP") map = MakeMove(0, -1);
        else if (action == "MOVE_DOWN") map = MakeMove(0, 1);
        else if (action == "MOVE_LEFT") map = MakeMove(-1, 0);
        else if (action == "MOVE_RIGHT") map = MakeMove(1, 0);
        else if (action == "ATTACK_UP") map = MakeAttack(0, -1);
        else if (action == "ATTACK_DOWN") map = MakeAttack(0, 1);
        else if (action == "ATTACK_LEFT") map = MakeAttack(-1, 0);
        else if (action == "ATTACK_RIGHT") map = MakeAttack(1, 0);
        else if (action == "TOGGLE_MODE") map = {CommandType::ToggleMode, 0, 0};
        else if (action == "CAST_SPELL") map = {CommandType::CastSpell, 0, 0};
        else if (action == "QUIT") map = {CommandType::Quit, 0, 0};
        else {
            continue;
        }

        used_keys.insert(key);
        used_actions.insert(action);
        parsed[key] = map;
    }

    if (!IsValidSet(parsed)) {
        return MakeDefault();
    }

    ControlBindings cb;
    cb.bindings_ = parsed;
    cb.using_default_ = false;
    return cb;
}

Command ControlBindings::MapInput(char key, const std::string& payload) const {
    auto it = bindings_.find(static_cast<char>(std::tolower(static_cast<unsigned char>(key))));
    if (it == bindings_.end()) return Command{};

    const auto& map = it->second;
    switch (map.type) {
        case CommandType::Move:
            return Command::MakeMove(map.dx, map.dy);
        case CommandType::Attack:
            return Command::MakeAttack(map.dx, map.dy);
        case CommandType::ToggleMode:
            return Command::MakeToggle();
        case CommandType::CastSpell: {
            int idx = -1;
            if (!payload.empty()) {
                try {
                    idx = std::stoi(payload);
                } catch (...) {
                    idx = -1;
                }
            }
            return Command::MakeCast(idx);
        }
        case CommandType::Quit:
            return Command::MakeQuit();
        default:
            break;
    }
    return Command{};
}

bool ControlBindings::IsUsingDefault() const {
    return using_default_;
}
