#pragma once
#include <string>

class SaveLoadException : public std::exception {
 protected:
  std::string message;

 public:
  explicit SaveLoadException(const std::string& msg);
  const char* what() const noexcept override;
};
