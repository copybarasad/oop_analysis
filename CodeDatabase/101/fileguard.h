#ifndef FILEGUARD_H
#define FILEGUARD_H
#include <fstream>

class FileGuard
{
private:
    std::fstream file;
    std::string filename;
    bool commited;
public:
    FileGuard(std::string name);
    ~FileGuard();
    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;
    FileGuard(FileGuard&& other) noexcept;
    FileGuard& operator =(FileGuard&& other) noexcept;
    std::fstream& get_file();
    void commit();
};

#endif // FILEGUARD_H
