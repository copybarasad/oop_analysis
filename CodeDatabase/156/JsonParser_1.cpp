//
// Created by Artem on 19.11.2025.
//

#include "JsonParser.h"
#include "../../Exceptions/UnexpectedBehaviorException.h"

std::vector<std::string> JsonParser::parseJsonArray(const std::string &arr) {
    if (arr[0] != '[' && arr[arr.size() - 1] != ']')
        throw UnexpectedBehaviorException("string not a json array");
    std::vector<std::string> res;
    std::string tmp;

    int nestedLevel = 0;
    for (int i = 1; i < arr.size()-1; ++i) {
        if(arr[i] == '{' || arr[i] == '['){
            nestedLevel++;
        }
        if(arr[i] == '}' || arr[i] == ']'){
            nestedLevel--;
        }
        if (arr[i] == ',' && nestedLevel == 0){
            res.push_back(tmp);
            tmp = "";
        }else{
            tmp += arr[i];
        }
    }
    if (!tmp.empty())
        res.push_back(tmp);
    return res;
}


std::map<std::string, std::string> JsonParser::parseJsonWithNestedObj(const std::string &json) {
    std::map<std::string, std::string> res;
    int nestedLevel = 0;

    if (json.at(0) != '{' && json.at(json.size() - 1) != '}'){
        throw UnexpectedBehaviorException("string not json object");
    }

    std::string key;
    std::string value;
    bool isKey = true;
    for (int i = 1; i < json.size(); ++i) {
        if (nestedLevel != 0){
            if(json[i] == '{' || json[i] == '['){
                nestedLevel++;
            }
            value += json[i];
            if (json[i] == '}' || json[i] == ']'){
                nestedLevel--;
            }


            if (nestedLevel == 0){
                isKey = true;
                res[key] = value;
                key = "";
                value = "";
            }
            continue;
        }

        if (isKey){
            //секция обработки key
            if (json[i] == '}' || json[i] == ',') //обрабатываем символы после nasted obj
                continue;
            if (json[i] == ':') {
                isKey = false;
                continue;
            }
            key += json[i];
        }else{
            //секция обработки value
            if (json[i] == ',' || json[i] == '}'){
                isKey = true;
                res[key] = value;
                key = "";
                value = "";
                continue;
            }

            if (json[i] == '{' || json[i] == '['){
                if (!value.empty())
                    throw UnexpectedBehaviorException("Error when parse nested obj or array, in value temp str not zero symbols!");
                nestedLevel = 1;
                value += json[i];
                continue;
            }
            value += json[i];
        }
    }
    if (key.empty() && !value.empty())
        throw UnexpectedBehaviorException("Error when parse in the end key.empty() && !value.empty()");
    if (!key.empty() && value.empty())
        throw UnexpectedBehaviorException("Error when parse in the end !key.empty() && value.empty()");

    return res;
}

EntityType JsonParser::extractEntityType(const std::string &jsonStr) {
    std::map<std::string, std::string> res = parseJsonWithNestedObj(jsonStr);
    if (res.count("type") != 0){
        return stringTypeToEnumEntityType(res.at("type"));
    }else
        throw UnexpectedBehaviorException("Can't extract EntityType, no field named \"type\" in object");
}


static const std::map<std::string, EntityType> stringToEnumEntityType = {
        {"PlayerEnt",EntityType::PlayerEnt},
        {"EnemyEnt", EntityType::EnemyEnt},
        {"EnemySpawnerBuildingEnt", EntityType::EnemySpawnerBuildingEnt},
        {"Ally", EntityType::Ally},
        {"EnemyDefenceTower", EntityType::EnemyDefenceTower}
};
EntityType JsonParser::stringTypeToEnumEntityType(const std::string& type) {
    if(stringToEnumEntityType.count(type) != 0){
        return stringToEnumEntityType.at(type);
    }else
        throw UnexpectedBehaviorException("Can't convert stringType ToEnumEntityType because no type named like this");
}

static const std::map<EntityType, std::string> enumEntityTypeToString = {
        {EntityType::PlayerEnt, "PlayerEnt"},
        {EntityType::EnemyEnt, "EnemyEnt"},
        {EntityType::EnemySpawnerBuildingEnt, "EnemySpawnerBuildingEnt"},
        {EntityType::Ally, "Ally"},
        {EntityType::EnemyDefenceTower, "EnemyDefenceTower"}
};
std::string JsonParser::enumEntityTypeToStringType(EntityType type) {
    if(enumEntityTypeToString.count(type) != 0){
        return enumEntityTypeToString.at(type);
    }else
        throw UnexpectedBehaviorException("Can't convert enumEntityType ToStringType because has no this type in ap[p!!!");
}
