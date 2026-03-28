#ifndef UTILS_TEMPLATE_HPP
#define UTILS_TEMPLATE_HPP

#include <cstdarg>
#include <string>

class Template {
  public:
    Template(const char *fmt);
    std::string populate(...);

  private:
    const char *m_Fmt;
};

#endif
