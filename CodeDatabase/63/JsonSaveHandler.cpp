#include "JsonSaveHandler.h"

JsonSaveHandler::JsonSaveHandler(const std::string& filename) {
    file.open(filename, std::ios::out);
    if (!file.is_open()) {
        file.clear();
        throw std::runtime_error("Не удалось открыть файл " + filename + " для записи");
    }
}

JsonSaveHandler::~JsonSaveHandler() {
    if (file.is_open())
        file.close();
}

bool JsonSaveHandler::save(const json& data) {
    file << data.dump(4);
    return true;
}