#ifndef SAVE_LOAD_EXCEPT_H
#define SAVE_LOAD_EXCEPT_H

#include <string>
#include <stdexcept>

class SaveLoadException : public std::runtime_error {
    public:
    SaveLoadException (const std::string& message);
};

#endif