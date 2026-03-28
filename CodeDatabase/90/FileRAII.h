#ifndef RPG_FILE_RAII_H_
#define RPG_FILE_RAII_H_

#include <fstream>
#include <stdexcept>

class FileRAII {
private:
  std::fstream file;
  std::string filename;

public:
  FileRAII(const std::string &filename, std::ios_base::openmode mode)
      : filename(filename) {
    file.open(filename, mode);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open file: " + filename);
    }
  }
  FileRAII(const FileRAII &) = delete;
  FileRAII &operator=(const FileRAII &) = delete;
  FileRAII(FileRAII &&other) noexcept
      : file(std::move(other.file)), filename(std::move(other.filename)) {}
  FileRAII &operator=(FileRAII &&other) noexcept {
    if (this != &other) {
      if (file.is_open()) {
        file.close();
      }
      file = std::move(other.file);
      filename = std::move(other.filename);
    }
    return *this;
  }
  ~FileRAII() {
    if (file.is_open()) {
      file.close();
    }
  }
  std::fstream &get() { return file; }
  const std::fstream &get() const { return file; }
  operator bool() const { return file.good(); }
  bool isOpen() const { return file.is_open(); }
  const std::string &getFilename() const { return filename; }
  void close() {
    if (file.is_open()) {
      file.close();
    }
  }
  std::string readLine() {
    std::string line;
    if (!std::getline(file, line)) {
      throw std::runtime_error("Error reading from file: " + filename);
    }
    return line;
  }
  void writeLine(const std::string &line) {
    file << line << "\n";
    if (file.fail()) {
      throw std::runtime_error("Error writing to file: " + filename);
    }
  }
};

#endif // RPG_FILE_RAII_H_