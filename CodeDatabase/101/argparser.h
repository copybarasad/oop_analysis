#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <string>
#include <vector>
#include <map>
#include <unordered_set>

class ArgParser {
private:
    std::map<std::string, std::string> options;
    std::unordered_set<std::string> flags;
    std::vector<std::string> positionalArgs;

public:
    ArgParser(int argc, char* argv[]);

    // Проверка наличия опции
    bool hasOption(const std::string& name) const;

    // Получение значения опции
    std::string getOption(const std::string& name, const std::string& defaultValue = "") const;

    // Проверка наличия флага
    bool hasFlag(const std::string& flag) const;

    // Получение позиционных аргументов
    const std::vector<std::string>& getPositionalArgs() const;

    // Вспомогательные методы
    void printHelp() const;

private:
    void parse(int argc, char* argv[]);
    bool isOption(const std::string& arg);
    bool isFlag(const std::string& arg);
};

#endif // ARGPARSER_H
