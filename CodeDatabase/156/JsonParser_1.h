//
// Created by Artem on 19.11.2025.
//

#ifndef LABAOOP2_JSONPARSER_H
#define LABAOOP2_JSONPARSER_H


#include <string>
#include <map>
#include <vector>
#include "../../Entities/EntityType.h"

class JsonParser {
public:
    static std::map<std::string, std::string> parseJsonWithNestedObj(const std::string& json);
    static EntityType extractEntityType(const std::string& jsonStr);
    static std::vector<std::string> parseJsonArray(const std::string &arr);
    static EntityType stringTypeToEnumEntityType(const std::string& type);
    static std::string enumEntityTypeToStringType(EntityType type);
};


#endif //LABAOOP2_JSONPARSER_H
