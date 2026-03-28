#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <iomanip>
#include <utility>

class SaveFileProtector 
{
private:
    static const std::string HASH_MARKER;
    
    static std::string calculateHash(const std::string& content);
    
public:
    static bool protectFile(const std::string& filename, const std::string& data);
    static std::pair<bool, std::string> readProtectedFile(const std::string& filename);
};