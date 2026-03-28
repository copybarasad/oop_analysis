#pragma once
#include "Serialization/FileReader.hpp"
#include "Serialization/FileWriter.hpp"
#include "Serialization/Exceptions.hpp"
#include <json.hpp>
#include <string>


class JsonFileManager {
public:
    nlohmann::json read(const char* path);
    void write(const char* path, nlohmann::json& data);
};
