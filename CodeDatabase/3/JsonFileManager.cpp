#include "Serialization/JsonFileManager.hpp"


nlohmann::json JsonFileManager::read(const char* path) {
    FileReader reader(path);
    std::string text = reader.read();
    try {
        return nlohmann::json::parse(text);
    } catch (const std::exception& e) {
        throw JsonParseException(path, e.what());
    }
}


void JsonFileManager::write(const char* path, nlohmann::json& data) {
    FileWriter writer(path);
    std::string text = data.dump(4);

    writer.write(text);
}