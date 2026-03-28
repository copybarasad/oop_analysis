#include "JsonLoadHandler.h"


JsonLoadHandler::JsonLoadHandler(const std::string& filename) {
    file.open(filename, std::ios::in);
    if (!file.is_open()) {
        file.clear();
        throw std::runtime_error("Не удалось открыть файл " + filename + " для чтения");
    }
}

JsonLoadHandler::~JsonLoadHandler() {
    if (file.is_open())
        file.close();
}

json JsonLoadHandler::load() {
    try {
        return json::parse(file);
    } catch (const json::exception &e) {
        throw std::runtime_error("Ошибка парсинга json файла");
    }
}