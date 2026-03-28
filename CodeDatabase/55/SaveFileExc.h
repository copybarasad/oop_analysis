#pragma once
#include <stdexcept>


class SaveFileOpenError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class SaveFileWriteError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};