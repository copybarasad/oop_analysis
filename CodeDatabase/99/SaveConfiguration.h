#ifndef SAVE_CONFIGURATION_H
#define SAVE_CONFIGURATION_H

#include <string>

// Configuration class for save system
// Encapsulates save directory and file format settings
class SaveConfiguration {
private:
	std::string saveDirectory;
	std::string saveExtension;
	int saveVersion;

public:
	SaveConfiguration();
	explicit SaveConfiguration(const std::string& directory);

	// Getters
	const std::string& getSaveDirectory() const;
	const std::string& getSaveExtension() const;
	int getSaveVersion() const;

	// Build full file path
	std::string buildFilePath(const std::string& saveName) const;
};

#endif // SAVE_CONFIGURATION_H