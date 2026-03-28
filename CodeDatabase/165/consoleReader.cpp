#include "consoleReader.h"

command consoleReader::parseCommand(const std::string &com){
        std::stringstream ss(com);
        std::string segment;
        std::vector<std::string> parts;

        while (std::getline(ss, segment, ' ')){
            if(!segment.empty()) {
                parts.push_back(segment);
            }
        }

        if (parts.empty()) {
            return command(std::string("None"), {}, false);
        }

        std::string command_name = parts[0];
        std::vector<std::string> arguments(parts.begin() + 1, parts.end());

        return command(command_name, arguments, true);
}
