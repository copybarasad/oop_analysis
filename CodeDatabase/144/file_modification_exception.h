#ifndef FILE_MODIFICATION_EXCEPTION_H
#define FILE_MODIFICATION_EXCEPTION_H

#include "load_game_exception.h"
#include <string>
#include <cstdint>

class FileModificationException : public LoadGameException {
public:
	explicit FileModificationException(const std::string& path, 
		uint64_t expectedChecksum, uint64_t actualChecksum)
		: LoadGameException("Save file integrity check failed: " + path + 
			" has been modified or corrupted. Expected checksum: " + 
			std::to_string(expectedChecksum) + ", actual checksum: " + 
			std::to_string(actualChecksum) + ". File may have been tampered with."),
		filePath(path),
		expectedChecksumValue(expectedChecksum),
		actualChecksumValue(actualChecksum) {}

	const std::string& getFilePath() const { return filePath; }
	uint64_t getExpectedChecksum() const { return expectedChecksumValue; }
	uint64_t getActualChecksum() const { return actualChecksumValue; }

private:
	std::string filePath;
	uint64_t expectedChecksumValue;
	uint64_t actualChecksumValue;
};

#endif

