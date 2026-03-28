#ifndef PRINTABLE_HPP
#define PRINTABLE_HPP

#include <string>

#include "color.hpp"

class Printable {
  public:
    virtual std::string chr() = 0;
    virtual Color color() = 0;
};

#endif /* PRINTABLE_HPP */
