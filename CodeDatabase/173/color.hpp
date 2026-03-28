#ifndef COLOR_HPP
#define COLOR_HPP

#include <string>

class Color {
  public:
    // Regular colors.
    static Color red();
    static Color green();
    static Color white();

    // High intensity.
    static Color lime();
    static Color magenta();

    // Utility escape codes.
    static Color reset();

    std::string ansiEscapeCode();

  private:
    Color(const std::string &ansiCode);
    std::string m_AnsiEscapeCode;
};

#endif /* COLOR_HPP */
