#ifndef FILENOTFOUNDEXCEPTION_H
#define FILENOTFOUNDEXCEPTION_H

#include "LoadGameException.h"

class FileNotFoundException : public LoadGameException {
public:
    explicit FileNotFoundException(const std::string& msg) : LoadGameException(msg) {}
};

#endif