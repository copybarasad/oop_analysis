#include "controllers/command.h"
#include <unordered_map>

class KeyMapper{
    public:
        static std::unordered_map<char, CommandType> loadConfig(const std::string& filename);
        static std::unordered_map<char, CommandType> getDefaultMap();
        static bool validateConfig(std::unordered_map<char, CommandType>& map);
    
};