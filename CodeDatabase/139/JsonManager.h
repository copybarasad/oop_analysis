#pragma once
#include "Interfaces/GameBoard.h"
#include "JsonLib/json.hpp"
#include <fstream>


class JsonManager
{
private:
    std::string filename;

    class FileInputWrapper {
        private:
            std::ofstream file;
        public:
            FileInputWrapper(const std::string& filename);

            std::ofstream& get();

            ~FileInputWrapper();
    };

    class FileOutputWrapper {
        private:
            std::ifstream file;
        public:
            FileOutputWrapper(const std::string& filename);

            std::ifstream& get();

            ~FileOutputWrapper();
    };
public:
    JsonManager(const std::string& filename);

    void jsonDump(const GameBoard& board, unsigned short level);

    nlohmann::json jsonLoad();
private:
    nlohmann::json dumpGameBoard(const GameBoard& board, unsigned short level);
};