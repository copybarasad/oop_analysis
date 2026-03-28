#include "TextGifProtector.h"

const std::string TextGifProtector::HASH_MARKER = "TEXTGIF_HASH";

std::string TextGifProtector::calculateHash(const std::string& content) 
{
    std::hash<std::string> hasher;
    size_t hashValue = hasher(content);
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hashValue;
    return ss.str();
}

bool TextGifProtector::hasHash(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    std::string firstLine;
    std::getline(file, firstLine);
    file.close();
    return (firstLine.find(HASH_MARKER) == 0);
}

std::string TextGifProtector::extractContent(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()) return "";
    std::stringstream content;
    std::string line;
    bool hashSkipped = false;
    while (std::getline(file, line)) 
    {
        if (!hashSkipped && line.find(HASH_MARKER) == 0) 
        {
            hashSkipped = true;
            continue;
        }
        content << line << "\n";
    }
    file.close();
    return content.str();
}

bool TextGifProtector::protectFile(const std::string& filename) 
{
    if (hasHash(filename)) return true;
    
    std::string content = extractContent(filename);
    if (content.empty()) return false;
    
    std::string hash = calculateHash(content);
    
    std::string backupName = filename + ".backup";
    std::ifstream src(filename, std::ios::binary);
    std::ofstream dst(backupName, std::ios::binary);
    if (src && dst) dst << src.rdbuf();
    src.close();
    dst.close();
    
    std::ofstream outFile(filename);
    if (!outFile.is_open()) return false;
    outFile << HASH_MARKER << "=" << hash << "\n";
    outFile << content;
    outFile.close();
    return true;
}

bool TextGifProtector::repairGifFiles(bool repair_flag) 
{
    if (!repair_flag) return true;
    
    std::vector<std::string> gifFiles;
    try 
    {
        for (const auto& entry : std::filesystem::directory_iterator(".")) 
        {
            if (entry.is_regular_file()) 
            {
                std::string filename = entry.path().filename().string();
                if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".txt") 
                {
                    size_t underscore_pos = filename.find('_');
                    size_t dot_pos = filename.find('.');
                    if (underscore_pos != std::string::npos && dot_pos != std::string::npos && underscore_pos < dot_pos) 
                    {
                        gifFiles.push_back(filename);
                    }
                }
            }
        }
    } 
    catch (...) 
    {
        return false;
    }
    
    if (gifFiles.empty()) return true;
    for (const auto& file : gifFiles) protectFile(file);
    return true;
}

std::string TextGifProtector::readProtectedFile(const std::string& filename) 
{
    return extractContent(filename);
}