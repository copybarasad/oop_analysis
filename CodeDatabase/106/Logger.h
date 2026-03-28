#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>
#include <fstream>

class Logger {
private:
    std::vector<std::string> logs; // хранение всех сообщений
    std::string filename;
    std::ofstream file;

    bool openFile() {
        file.open(filename, std::ios::app);
        return file.is_open();
    }

    Logger(const std::string& file = "game.log") : filename(file) { openFile(); }

    // запрет копирования и присваивания соотв
    // см ниже почему (singleton)
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

public:
    // здесь реализация singleton'а. статик обоснован именно
    // этим, иначе каждый вызов создавал бы новый логгер, как итог
    // просто нет смысла от паттерна. всегда обращаемся
    // к ОДНОМУ и тому же Logger::getInstance()
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    // деструктор логгера, чтобы не потерять лишних данных при закрытии
    ~Logger() {
        if (file.is_open()) {
            file.close();
        }
    }

    // сеттер файла (логично же из названия, да?)
    void setFile(const std::string& file_name) {
        // закрываем старый файл, если открыт
        if (file.is_open()) {
            file.close();
        }
        filename = file_name;
        openFile();
    }

    // добавление лога
    void add(const std::string& message) {
        logs.push_back(message);

        if (file.is_open()) {
            file << message << "\n";
            file.flush(); // сброс буфера на всяк
        }
    }

    // геттер всех логов
    const std::vector<std::string>& getAll() const {
        return logs;
    }

    // геттер последнего лога
    std::vector<std::string> getLast(int count) const {
        if (logs.size() <= count) {
            return logs;
        }
        return std::vector<std::string>(logs.end() - count, logs.end());
    }

    // сохранение всех логов в указанный файл
    bool saveToFile(const std::string& save_filename) {
        // открываем файл для записи
        std::ofstream save_file(save_filename);
        if (!save_file.is_open()) {
            return false;
        }

        // вписываем каждое сообщ
        for (const auto& log : logs) {
            save_file << log << "\n";
        }

        save_file.close();
        return true;
    }

    // коротко и ясно: чистка логов
    void clear() {
        logs.clear();
    }

    // коротко и ясно 2: размер логов
    size_t size() const {
        return logs.size();
    }

    // коротко и ясно 3: пустота лога
    bool empty() const {
        return logs.empty();
    }
};

#endif