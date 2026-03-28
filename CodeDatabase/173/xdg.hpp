/*
 * SEE: https://specifications.freedesktop.org/basedir/latest/
 * */

#ifndef UTILS_XDG_HPP
#define UTILS_XDG_HPP

#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

class XDG {
  public:
    XDG &inDataHome();
    XDG &inConfigHome();
    XDG &withAppName(std::string name);
    XDG &withFileName(std::string name);

    fs::path build();

  private:
    std::string m_Base;
    std::string m_AppName;
    std::string m_FileName;

    fs::path prefix();
};

#endif /* UTILS_XDG_HPP */
