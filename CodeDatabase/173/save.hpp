#ifndef SAVE_HPP
#define SAVE_HPP

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>

namespace fs = std::filesystem;

#include "../save/dumpable.hpp"
#include "exceptions.hpp"

class Save {
  public:
    Save();
    Save(bytedump bytes);
    Save(fs::path src);

    void write(bytedump bytes);
    std::vector<bytedump> parts();
    bytedump bytes();

    void save(fs::path dst);

  private:
    bytedump m_Bytes;

    bytedump boundary();
    bytedump signature();
    void insertBoundary();
    void insertSignature();
    void validateSignature();
};

#endif /* SAVE_HPP */
