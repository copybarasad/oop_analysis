#ifndef VALIDATION_EXCEPT_H
#define VALIDATION_EXCEPT_H

#include "../SaveLoadException/SaveLoadException.hpp"

class ValidationException : public SaveLoadException {
    public:
    ValidationException (const std::string& message);
};

#endif