#include "template.hpp"

Template::Template(const char *fmt) : m_Fmt(fmt) {}

std::string Template::populate(...) {
    va_list args;
    va_start(args, m_Fmt);

    va_list args_copy;
    va_copy(args_copy, args);
    int size = std::vsnprintf(nullptr, 0, m_Fmt, args_copy);
    va_end(args_copy);

    if (size < 0) {
        va_end(args);
        return "";
    }

    std::string result(size, '\0');
    std::vsnprintf(&result[0], size + 1, m_Fmt, args);
    va_end(args);

    return result;
}
