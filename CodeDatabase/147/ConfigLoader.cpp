#include "ConfigLoader.h"
#include <fstream>
#include <sstream>
#include <algorithm>

std::unordered_map<char, std::string> ConfigLoader::loadBindings(const std::string& path) const {
    std::unordered_map<char, std::string> result;
    std::ifstream in(path);
    if (!in.is_open()) {
        return {};
    }

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        auto pos = line.find('=');
        if (pos == std::string::npos || pos == 0 || pos == line.size() - 1) {
            return {};
        }
        char key = static_cast<char>(std::toupper(line[0]));
        std::string action = line.substr(pos + 1);
        // убираем пробелы
        action.erase(std::remove_if(action.begin(), action.end(), ::isspace), action.end());
        result[key] = action;
    }

    if (!validate(result)) {
        return {};
    }
    return result;
}

bool ConfigLoader::validate(const std::unordered_map<char, std::string>& bindings) const {
    // Требуем полный набор уникальных соответствий
    const std::set<std::string> required = {
        "UP", "DOWN", "LEFT", "RIGHT",
        "TOGGLE_MODE", "CAST_SPELL", "RANGED_ATTACK",
        "BUY_SPELL", "SAVE", "LOAD", "EXIT"
    };
    std::set<std::string> seenActions;
    std::set<char> seenKeys;
    for (const auto& kv : bindings) {
        if (seenKeys.count(kv.first)) return false;
        seenKeys.insert(kv.first);
        seenActions.insert(kv.second);
    }
    // должен быть полный набор и без дубликатов
    return seenActions == required;
}
