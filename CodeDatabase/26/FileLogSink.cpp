#include "FileLogSink.hpp"

FileLogSink::FileLogSink(const std::string& path)
	: file_(path, std::ios::app) {}

void FileLogSink::write(const std::string& message) {
	if (file_) {
		file_ << message << std::endl;
	}
}

bool FileLogSink::isOpen() const noexcept {
	return static_cast<bool>(file_);
}
