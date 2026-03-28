#pragma once

#include "SaveLoadException.h"

class FileReadException : public SaveLoadException {
 public:
  explicit FileReadException(const std::string& filename);
};
