#ifndef FILE_HPP
#define FILE_HPP
#include <iostream>
#include <fstream>
#include "gameexc.hpp"

class saveFile {
    std::ifstream file;

public:
    saveFile(std::string& path);
    std::ifstream& get();
    ~saveFile(){file.close();}
};

#endif