#include "ArgParser.h"
#include <iostream>
#include <sstream>

ArgParser::ArgParser(int argc, char* argv[]) {
    parse(argc, argv);
}

void ArgParser::parse(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        // Обработка флагов (--help, --console, --nolog)
        if (isFlag(arg)) {
            flags.insert(arg);
        }
        // Обработка опций с параметрами (--log=file.txt, --loglevel=debug)
        else if (isOption(arg)) {
            size_t equalPos = arg.find('=');
            if (equalPos != std::string::npos) {
                std::string name = arg.substr(0, equalPos);
                std::string value = arg.substr(equalPos + 1);
                options[name] = value;
            } else if (i + 1 < argc) {
                // Проверяем следующий аргумент - если это не новая опция/флаг
                std::string nextArg = argv[i + 1];
                if (!isFlag(nextArg) && !isOption(nextArg)) {
                    options[arg] = nextArg;
                    ++i; // Пропускаем следующий аргумент, так как он значение
                } else {
                    // Опция без значения
                    options[arg] = "";
                }
            } else {
                // Опция без значения в конце
                options[arg] = "";
            }
        }
        // Позиционные аргументы
        else {
            positionalArgs.push_back(arg);
        }
    }
}

bool ArgParser::isOption(const std::string& arg) {
    // Опции начинаются с -- или -
    return (arg.size() > 2 && arg.substr(0, 2) == "--") ||
           (arg.size() > 1 && arg[0] == '-' && arg[1] != '-');
}

bool ArgParser::isFlag(const std::string& arg) {
    // Флаги - это опции без значений
    return arg == "--help" || arg == "--console" || arg == "--nolog";
}

bool ArgParser::hasOption(const std::string& name) const {
    return options.find(name) != options.end();
}

std::string ArgParser::getOption(const std::string& name, const std::string& defaultValue) const {
    auto it = options.find(name);
    if (it != options.end()) {
        return it->second;
    }
    return defaultValue;
}

bool ArgParser::hasFlag(const std::string& flag) const {
    return flags.find(flag) != flags.end();
}

const std::vector<std::string>& ArgParser::getPositionalArgs() const {
    return positionalArgs;
}

void ArgParser::printHelp() const {
    std::cout << "Usage: game [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --help                Show this help message\n";
    std::cout << "  --console             Enable console logging\n";
    std::cout << "  --nolog               Disable all logging\n";
    std::cout << "  --log=FILENAME        Log to file\n";
    std::cout << "  --loglevel=LEVEL      Log level (debug, info, warning, error)\n";
    std::cout << "                        Default: info\n\n";
    std::cout << "Examples:\n";
    std::cout << "  game --console --loglevel=debug\n";
    std::cout << "  game --log=game.log --loglevel=info\n";
    std::cout << "  game --console --log=game.log\n";
}
