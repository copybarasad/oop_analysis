//
// Created by Mac on 02.12.2025.
//

#ifndef GAME_TERM_CONFIGLOADER_H
#define GAME_TERM_CONFIGLOADER_H
#include <map>

#include "../commands/Command.h"


class ConfigLoader {
    static CommandType stringToCommand(const std::string& str);
    static bool validateConfig(const std::map<char, CommandType>& config);

public:
    static std::map<char, CommandType> loadFromFile(const std::string& filename);
};


#endif //GAME_TERM_CONFIGLOADER_H