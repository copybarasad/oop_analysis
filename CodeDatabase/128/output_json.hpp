#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

class JsonWriter
{      
public:
    JsonWriter();
    explicit JsonWriter(const std::string& path);

    void Open(const std::string& path);
    void Write(const nlohmann::json& j);

    ~JsonWriter();

private:
    std::ofstream output_stream_;   
    std::string   file_path_;    
};
