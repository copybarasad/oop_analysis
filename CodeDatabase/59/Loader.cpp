#include "Loader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Loader::Loader(const std::string& saveDir) : saveDirectory(saveDir) {
    try {
        std::filesystem::create_directory(saveDirectory);
    } catch (const std::exception& e) {
        std::cout << "Ошибка: Не удалось создать папку для сохранений."
        << std::endl;
    }
}

std::string Loader::getSaveFilePath(int slot) const {
    return saveDirectory + "/save_" + std::to_string(slot) + ".json";
}

long Loader::calculateChecksum(const std::string& content) {
    long sum = 0;
    for (char c : content) {
        sum = (sum * 12 + c) % 123123123;
    }
    return sum;
}

bool Loader::doSavesExist() const {
    try {
        for (const auto& entry : std::filesystem::directory_iterator(saveDirectory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                return true;
            }
        }
    } catch (...) {
        return false;
    }
    return false;
}

bool Loader::saveGame(int slot, const GameData& data) {
    std::string content;
    try {
        json j = data.toJson();
        content = j.dump(4);
    } catch (const std::exception& e) {
        std::cout << "Ошибка: Не удалось сериализовать состояние игры."
        << std::endl;
        return false;
    }

    unsigned long checksum = calculateChecksum(content);

    std::ofstream file(getSaveFilePath(slot));
    if (!file.is_open()) {
        std::cout << "Ошибка: Не удалось открыть файл сохранения для записи: " << getSaveFilePath(slot) << std::endl;
        return false;
    }

    file << content;
    file << "\n" << checksum << "\n";
    file.close();

    return true;
}

bool Loader::loadGame(int slot, GameData& data) {
    std::ifstream file(getSaveFilePath(slot));
    if (!file.is_open()) {
        std::cout << "Ошибка: Файл сохранения не найден или не может быть открыт: " << getSaveFilePath(slot) << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string fullContent = buffer.str();
    file.close();

    size_t lastNewline = fullContent.find_last_of('\n');
    size_t secondLastNewline = fullContent.find_last_of('\n', lastNewline - 1);
    
    if (secondLastNewline == std::string::npos || lastNewline == std::string::npos) {
        std::cout << "Ошибка: Файл сохранения поврежден (отсутствует контрольная сумма)."
        << std::endl;
        return false;
    }

    std::string content = fullContent.substr(0, secondLastNewline);
    std::string checksumStr = fullContent.substr(secondLastNewline + 1, (lastNewline - secondLastNewline) - 1);

    unsigned long readChecksum;
    try {
        readChecksum = std::stoul(checksumStr);
    } catch (...) {
        std::cout << "Ошибка: Файл сохранения поврежден (неверный формат контрольной суммы)."
        << std::endl;
        return false;
    }
    
    if (readChecksum != calculateChecksum(content)) {
        std::cout << "Ошибка: Несовпадение контрольной суммы! Файл сохранения поврежден."
        << std::endl;
        return false;
    }

    try {
        json j = json::parse(content);
        data.fromJson(j);

    } catch (...) {
        std::cout << "Ошибка: Не удалось разобрать файл сохранения."
        << std::endl;
        return false;
    }

    return true;
}