#ifndef CONTROL_LOADER_H
#define CONTROL_LOADER_H

#include "control_mapping.h"
#include <string>

class ControlLoader {
public:
    static ControlMapping loadFromFile(const std::string& filename);
    static void saveDefaultToFile(const std::string& filename);
};

#endif