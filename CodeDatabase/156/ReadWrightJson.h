//
// Created by Artem on 20.11.2025.
//

#ifndef LABAOOP2_READWRIGHTJSON_H
#define LABAOOP2_READWRIGHTJSON_H

#include <string>

class ReadWrightJson {
public:
    static std::string read(const std::string& fileName);
    static void write(const std::string& json, const std::string& fileName);
};


#endif //LABAOOP2_READWRIGHTJSON_H
