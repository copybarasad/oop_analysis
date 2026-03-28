#ifndef LOG_H
#define LOG_H
#include <iostream>
#include <fstream>
#include <string>
#include "logger.h"

class Log {
private:
    inline static Logger logger;

public:
    static void init(bool toConsole, bool toFile) {
        logger.useConsole = toConsole;
        logger.useFile = toFile;

        if (logger.fileStream.is_open()) logger.fileStream.close();

        if (toFile) {
            logger.fileStream.open("game_log.txt");
            if (!logger.fileStream.is_open()) {
                std::cerr << "Не удалось открыть файл логов!\n";
                logger.useFile = false;
                logger.useConsole = true;
            }
        }
    }

    static Logger* getStream() {
        return &logger;
    }

    static bool isFileOutput() {
        return logger.useFile;
    }

    static void close() {
        if (logger.fileStream.is_open()) {
            logger.fileStream.close();
        }
    }
};

#endif