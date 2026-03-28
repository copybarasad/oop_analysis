#pragma once
#include <string>
#include <stdexcept>

class FileException {
public:
    FileException(std::string m) {
        msg = m;
    }

    const char* what() {
        return msg.c_str();
    }
protected:
    std::string msg;
};


class KeyConfigException : public FileException {
public:
    KeyConfigException(std::string detail) : FileException("Invalid keyboards config: " + detail) {}
};

class FileReadException : public FileException {
public:
    FileReadException(std::string path) : FileException("Cannot read file: " + path) {}
};


class FileWriteException : public FileException {
public:
    FileWriteException(std::string path) : FileException("Cannot write file: " + path) {}
};


class JsonParseException : public FileException {
public:
    JsonParseException(std::string path, std::string err): FileException("JSON parse error in '" + path + "': " + err) {}
};


class InvalidJson : public FileException {
public:
    InvalidJson(std::string where, std::string detail) : FileException("Invalid JSON in " + where + ": " + detail) {}
};