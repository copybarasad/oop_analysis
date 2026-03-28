#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <fstream>
#include <string>

class FileModel {
public:
    enum class Mode {
        Read,
        Write
    };

    FileModel(const std::string& filename, Mode mode);
    ~FileModel();

    std::fstream& stream();

    FileModel(const FileModel&) = delete;
    FileModel& operator=(const FileModel&) = delete;
    FileModel(FileModel&&) = default;
    FileModel& operator=(FileModel&&) = default;

private:
    std::fstream fileStream;
    Mode mode;
    std::string fileName;
};

#endif

