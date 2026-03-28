#ifndef BASE_DESER_H
#define BASE_DESER_H

#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

#include "modules/Services/CombatService/CombatService.hpp"
#include "modules/Services/SaveLoadServices/SaveLoadException/ValidationException/ValidationException.hpp"

class BaseDeserializer {
    public:
    std::vector<std::string> split (const std::string& str, char delimiter) const;
    std::unordered_map<std::string, std::string> parseKeyValuePairs (const std::string& data) const;
    void validateRequiredFields (const std::unordered_map<std::string, std::string>& data, const std::vector<std::string>& requiredFields) const;
    int parseInt (const std::string& str, const std::string& context) const;
    unsigned int parseUnsignedInt (const std::string& str, const std::string& context) const;
    float parseFloat (const std::string& str, const std::string& context) const;
    size_t parseSizeT (const std::string& str, const std::string& context) const;
    bool parseBool (const std::string& str, const std::string& context) const;
    CombatMode parseCombatMode (const std::string& str) const;
};

#endif