#include "input_json.hpp"
#include "errors.hpp"

JsonReader::JsonReader() = default;

JsonReader::JsonReader(const std::string& path) {
    Open(path);
}

void JsonReader::Open(const std::string& path) {
    file_path_ = path;
    input_stream_.open(path);
    if (!input_stream_.is_open())
        throw FileNotExistError(path);   
}

nlohmann::json JsonReader::Read() {
    nlohmann::json j;
    try {
        input_stream_ >> j;
    }
    catch (const nlohmann::json::exception& e) {
        throw CorruptedDataError(e.what());  
    }
    catch (const std::exception& e) {
        throw LoadError(e.what());         
    }
    catch (...) {
        throw LoadError("Unknown read error");
    }
    return j;
}

JsonReader::~JsonReader() {
    if (input_stream_.is_open())
        input_stream_.close();
}
