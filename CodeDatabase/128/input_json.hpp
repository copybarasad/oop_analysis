#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

class JsonReader
{
public:
    JsonReader();
    explicit JsonReader(const std::string& path);

    void Open(const std::string& path);
    nlohmann::json Read();
    ~JsonReader();

private:
    std::ifstream input_stream_;   
    std::string   file_path_;      
};

