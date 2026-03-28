#include "BaseDeserializer.hpp"

std::vector<std::string> BaseDeserializer::split (const std::string& str, char delimiter) const {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

std::unordered_map<std::string, std::string> BaseDeserializer::parseKeyValuePairs (const std::string& data) const {
    std::unordered_map<std::string, std::string> result;
    auto pairs = split(data, ',');
        
    for (const auto& pair : pairs) {
        auto keyValue = split(pair, ':');
        if (keyValue.size() == 2) {
            result[keyValue[0]] = keyValue[1];
        }
    }
    return result;
}

void BaseDeserializer::validateRequiredFields (const std::unordered_map<std::string, std::string>& data, const std::vector<std::string>& requiredFields) const {
    for (const auto& field : requiredFields) {
        auto it = data.find(field);
        if (it == data.end()) {
            throw ValidationException("Missing required field: " + field);
        }
        if (it->second.empty()) {
            throw ValidationException("Required field '" + field + "' is empty");
        }
    }
}

int BaseDeserializer::parseInt (const std::string& str, const std::string& context) const {
    std::istringstream iss(str);
    int value;

    if (!(iss >> value)) {
        throw ValidationException("Invalid integer for " + context + ": '" + str + "'");
    }

    char remaining;
    if (iss >> remaining) {
        throw ValidationException("Invalid integer format for " + context + ": '" + str + "'");
    }
    
    return value;
}

unsigned int BaseDeserializer::parseUnsignedInt (const std::string& str, const std::string& context) const {
    int value = parseInt(str, context);
    if (value < 0) {
        throw ValidationException("Negative value for " + context + ": '" + str + "'");
    }
    return static_cast<unsigned int>(value);
}

float BaseDeserializer::parseFloat (const std::string& str, const std::string& context) const {
    std::istringstream iss(str);
    float value;
    
    if (!(iss >> value)) {
        throw ValidationException("Invalid float for " + context + ": '" + str + "'");
    }
    
    char remaining;
    if (iss >> remaining) {
        throw ValidationException("Invalid float format for " + context + ": '" + str + "'");
    }
    
    return value;
}

size_t BaseDeserializer::parseSizeT (const std::string& str, const std::string& context) const {
    std::istringstream iss(str);
    size_t value;
    
    if (!(iss >> value)) {
        throw ValidationException("Invalid size_t for " + context + ": '" + str + "'");
    }
    
    char remaining;
    if (iss >> remaining) {
        throw ValidationException("Invalid size_t format for " + context + ": '" + str + "'");
    }
    
    return value;
}

bool BaseDeserializer::parseBool (const std::string& str, const std::string& context) const {
    if (str == "1" || str == "true" || str == "TRUE") {
        return true;
    }
    else if (str == "0" || str == "false" || str == "FALSE") {
        return false;
    }
    else {
        throw ValidationException("Invalid boolean for " + context + ": '" + str + "'");
    }
}

CombatMode BaseDeserializer::parseCombatMode (const std::string& str) const {
    if (str == "CLOSE") return CombatMode::CLOSE;
    if (str == "RANGED") return CombatMode::RANGED;
    throw ValidationException("Invalid combat mode: " + str);
}