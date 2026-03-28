#include "save_file_checksum.h"
#include "file_access_exception.h"
#include "corrupted_data_exception.h"
#include <fstream>
#include <algorithm>
#include <sstream>

uint64_t SaveFileChecksum::calculate(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open()) {
		throw FileAccessException(filePath, "open for checksum calculation");
	}
	
	file.seekg(0, std::ios::end);
	std::streampos fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	
	return calculateFromStream(file, 0, fileSize);
}

uint64_t SaveFileChecksum::calculateFromStream(std::ifstream& stream, 
		std::streampos startPos, std::streampos endPos) {
	std::streampos originalPos = stream.tellg();
	stream.seekg(startPos, std::ios::beg);
	
	uint64_t checksum = 0;
	constexpr size_t bufferSize = 4096;
	char buffer[bufferSize];
	
	std::streampos currentPos = startPos;
	while (currentPos < endPos) {
		std::streampos remaining = endPos - currentPos;
		size_t readSize = static_cast<size_t>(std::min(remaining, 
			static_cast<std::streampos>(bufferSize)));
		
		stream.read(buffer, readSize);
		size_t actualRead = static_cast<size_t>(stream.gcount());
		
		for (size_t i = 0; i < actualRead; ++i) {
			checksum += static_cast<unsigned char>(buffer[i]);
			checksum = (checksum << 1) | (checksum >> 63);
		}
		
		currentPos += actualRead;
		if (actualRead < readSize) {
			break;
		}
	}
	
	stream.seekg(originalPos, std::ios::beg);
	return checksum;
}

void SaveFileChecksum::writeToStream(std::ofstream& stream, uint64_t checksum) {
	if (!stream.good()) {
		throw std::runtime_error("Stream is not in good state for writing checksum");
	}
	stream.write(reinterpret_cast<const char*>(&checksum), sizeof(checksum));
	if (!stream.good()) {
		throw std::runtime_error("Failed to write checksum to stream");
	}
}

uint64_t SaveFileChecksum::readFromStream(std::ifstream& stream) {
	if (!stream.good()) {
		throw CorruptedDataException("stream state before reading checksum");
	}
	
	uint64_t checksum = 0;
	stream.read(reinterpret_cast<char*>(&checksum), sizeof(checksum));
	
	if (stream.fail() || stream.gcount() != sizeof(checksum)) {
		throw CorruptedDataException("checksum data (failed to read checksum value)");
	}
	
	return checksum;
}

