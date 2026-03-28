#pragma once
#include <stdexcept>
#include <string>

class SaveLoadException : public std::runtime_error {
public:
    SaveLoadException(const std::string& msg) : std::runtime_error(msg) {}
};

class FileOpenException : public SaveLoadException {
private:
    std::string filename;
public:
    FileOpenException(const std::string& fname)
        : SaveLoadException("Failed to open file: " + fname), filename(fname) {
    }

    const std::string& get_filename() const { return filename; }
};

class FileWriteException : public SaveLoadException {
private:
    std::string filename;
public:
    FileWriteException(const std::string& fname)
        : SaveLoadException("Failed to write to file: " + fname), filename(fname) {
    }

    const std::string& get_filename() const { return filename; }
};

class FileReadException : public SaveLoadException {
private:
    std::string filename;
public:
    FileReadException(const std::string& fname)
        : SaveLoadException("Failed to read from file: " + fname), filename(fname) {
    }

    const std::string& get_filename() const { return filename; }
};

class CorruptedDataException : public SaveLoadException {
public:
    CorruptedDataException(const std::string& details)
        : SaveLoadException("Corrupted save data: " + details) {
    }
};
