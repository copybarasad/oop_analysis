#pragma once
#include "field.h"
#include <fstream>
#include <memory>

class FileRAII {
    std::fstream file;
public:
    explicit FileRAII(const std::string& filename, std::ios::openmode mode);
    ~FileRAII();
    std::fstream& stream() { return file; }
};

struct SaveLoadException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

struct FileOpenException : public SaveLoadException {
    FileOpenException(const string& msg) : SaveLoadException(msg) {}
};

struct CorruptedSaveException : public SaveLoadException {
    CorruptedSaveException(const string& msg) : SaveLoadException(msg) {}
};

class GameCycle {
public:
    static void save(const Field& field, const string& filename = "savegame.dat");
    static unique_ptr<Field> load(const string& filename = "savegame.dat");
};