#pragma once
#include "SaveFileExc.h"
#include <vector>
#include <fstream>
#include <string>


class SaveFileRAII {
private:
    std::ofstream out;
    std::ifstream in;
    std::string final_path;
    std::string temp_path;
    bool for_writing;

    SaveFileRAII() = default;

public:
    explicit SaveFileRAII(const std::string& path): final_path(path), temp_path(path + ".tmp"), for_writing(true) {
        out.open(temp_path, std::ios::binary);
        if (!out.is_open()) {
            throw SaveFileOpenError("Невозможно открыть временный файл: " + temp_path);
        }
    }

    SaveFileRAII(const SaveFileRAII&) = delete;
    SaveFileRAII& operator=(const SaveFileRAII&) = delete;

    SaveFileRAII(SaveFileRAII&&) = default;
    SaveFileRAII& operator=(SaveFileRAII&&) = default;

    static SaveFileRAII load(const std::string& path) {
        SaveFileRAII file;
        file.final_path = path;
        file.for_writing = false;
        file.in.open(path, std::ios::binary);
        if (!file.in.is_open()) {
            throw SaveFileOpenError("Файл не найден или его не существует: " + path);
        }
        return file;
    }

    template<typename T>
    SaveFileRAII& write(const T& value) {
        if (!for_writing || !out.is_open()) {
            throw SaveFileWriteError("Попытка записи в файл сохранения, доступный только для чтения или закрытый");
        }
        out.write(reinterpret_cast<const char*>(&value), sizeof(T));
        if (!out.good()) {
            throw SaveFileWriteError("Запись отклонена");
        }
        return *this;
    }

    SaveFileRAII& write(const std::string& str) {
        size_t len = str.size();
        write(len);
        if (len > 0) {
            out.write(str.data(), static_cast<std::streamsize>(len));
            if (!out.good()) {
                throw SaveFileWriteError("Запись строки невозможно");
            }
        }
        return *this;
    }

    template<typename T>
    SaveFileRAII& read(T& value) {
        if (for_writing || !in.is_open()) {
            throw SaveFileOpenError("Попытка записи в файл сохранения, доступный только для чтения или закрытый");
        }
        in.read(reinterpret_cast<char*>(&value), sizeof(T));
        if (!in.good()) {
            throw SaveFileOpenError("Чтение невозможно");
        }
        return *this;
    }

    SaveFileRAII& read(std::string& str) {
        size_t len;
        read(len);
        if (len > 10000) {
            throw SaveFileOpenError("Слишком длинная строка для записи");
        }
        str.resize(len);
        if (len > 0) {
            in.read(&str[0], static_cast<std::streamsize>(len));
            if (!in.good()) {
                throw SaveFileOpenError("Чтение строки невозможно");
            }
        }
        return *this;
    }

    bool eof() const {
        return for_writing ? false : in.eof();
    }

    ~SaveFileRAII() {
        if (for_writing) {
            out.close();
            if (out.fail()) {
                std::remove(temp_path.c_str());
                return;
            }
            if (std::rename(temp_path.c_str(), final_path.c_str()) != 0) {
                std::remove(temp_path.c_str());
            }
        } else {
            in.close();
        }
    }
};