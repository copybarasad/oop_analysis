#ifndef SAVE_FILE_CHECKSUM_H
#define SAVE_FILE_CHECKSUM_H

#include <cstdint>
#include <string>
#include <fstream>

class SaveFileChecksum {
public:
	static uint64_t calculate(const std::string& filePath);
	static uint64_t calculateFromStream(std::ifstream& stream, std::streampos startPos, std::streampos endPos);
	static void writeToStream(std::ofstream& stream, uint64_t checksum);
	static uint64_t readFromStream(std::ifstream& stream);
};

#endif

