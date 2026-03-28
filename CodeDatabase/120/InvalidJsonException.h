#pragma once

#include "SaveLoadException.h"

class InvalidJsonException : public SaveLoadException {
 public:
  explicit InvalidJsonException(const std::string& details);
};
