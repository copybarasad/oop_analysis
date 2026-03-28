#pragma once

inline std::string EnsureJsonExtension(const std::string& filename) {
  if (filename.size() >= 5 && filename.substr(filename.size() - 5) == ".json") {
    return filename;
  }
  return filename + ".json";
}
