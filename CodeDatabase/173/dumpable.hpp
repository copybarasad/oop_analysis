#ifndef SAVE_DUMPABLE_HPP
#define SAVE_DUMPABLE_HPP

#include <cstddef>
#include <vector>

using bytedump = std::vector<std::byte>;

class Dumpable {
  public:
    virtual bytedump dump() = 0;
};

#endif /* SAVE_DUMPABLE_HPP */
