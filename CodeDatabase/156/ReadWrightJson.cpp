//
// Created by Artem on 20.11.2025.
//

#include "ReadWrightJson.h"
#include "../../Exceptions/UnexpectedBehaviorException.h"
#include <fstream>
#include <sstream>

std::string ReadWrightJson::read(const std::string& fileName) {
    std::ifstream ifs(fileName);
    if (!ifs.is_open()){
        throw UnexpectedBehaviorException("ReadRightJson: UnexpectedBehavior, can't open file");
    }
    std::stringstream buffer;
    buffer << ifs.rdbuf();

    if (ifs.fail() && !ifs.eof()) {
        throw UnexpectedBehaviorException("ReadRightJson: UnexpectedBehavior, error reading file: " + fileName);
    }

    return buffer.str();
}

void ReadWrightJson::write(const std::string& json, const std::string& fileName) {
    std::ofstream ofs(fileName);
    if (!ofs.is_open() || ofs.fail()){
        throw UnexpectedBehaviorException("ReadRightJson: UnexpectedBehavior, can't open file");
    }
    ofs << json;
    if (ofs.fail()) {
        throw UnexpectedBehaviorException("ReadRightJson: UnexpectedBehavior, error writing to file: " + fileName);
    }
    ofs.flush();
    if (ofs.fail()) {
        throw UnexpectedBehaviorException("ReadRightJson: UnexpectedBehavior, error flushing data to file: " + fileName);
    }

}
