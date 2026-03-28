#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <functional>
#include <iomanip>

class TextGifProtector 
{
private:
    static const std::string HASH_MARKER;
    
    static std::string calculateHash(const std::string& content);
    static bool hasHash(const std::string& filename);
    static std::string extractContent(const std::string& filename);
    
public:
    static bool protectFile(const std::string& filename);
    static bool repairGifFiles(bool repair_flag);
    static std::string readProtectedFile(const std::string& filename);
};