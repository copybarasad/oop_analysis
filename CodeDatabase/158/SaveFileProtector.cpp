#include "SaveFileProtector.h"

const std::string SaveFileProtector::HASH_MARKER = "SAVEGAME_HASH";

std::string SaveFileProtector::calculateHash(const std::string& content) 
{
    std::hash<std::string> hasher;
    size_t hashValue = hasher(content);
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hashValue;
    std::string salt = "labanomer4";
    size_t saltedHash = hasher(content + salt);
    ss << std::hex << std::setw(16) << std::setfill('0') << saltedHash;
    return ss.str();
}

bool SaveFileProtector::protectFile(const std::string& filename, const std::string& data) 
{
    std::string hash = calculateHash(data);
    
    std::ofstream outFile(filename);
    if (!outFile.is_open()) return false;
    outFile << HASH_MARKER << "=" << hash << "\n";
    outFile << data;
    outFile.close();
    return true;
}

std::pair<bool, std::string> SaveFileProtector::readProtectedFile(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()) return {false, ""};
    
    std::string firstLine;
    std::getline(file, firstLine);
    
    if (firstLine.find(HASH_MARKER) != 0) 
    {
        file.close();
        return {false, ""};
    }
    
    std::string storedHash = firstLine.substr(firstLine.find('=') + 1);
    
    std::stringstream content;
    content << file.rdbuf();
    file.close();
    
    std::string fileContent = content.str();
    std::string calculatedHash = calculateHash(fileContent);
    
    if (storedHash != calculatedHash) 
    {
        std::cerr << "Save file corrupted! Hash mismatch.\n";
        return {false, ""};
    }
    
    return {true, fileContent};
}