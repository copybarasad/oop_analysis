#ifndef GAME_SAVE_MANAGER_H
#define GAME_SAVE_MANAGER_H

#include "SaveConfiguration.h"
#include <string>
#include <fstream>
#include <memory>
#include <vector>
#include <stdexcept>

// Forward declarations
class Game;
class Player;
class Field;
class Enemy;

// RAII wrapper for file operations
class FileGuard {
private:
	std::fstream file;
	std::string filename;
	bool isOpen;

public:
	FileGuard(const std::string& filename, std::ios::openmode mode);
	~FileGuard();

	// Disable copy
	FileGuard(const FileGuard&) = delete;
	FileGuard& operator=(const FileGuard&) = delete;

	// Allow move
	FileGuard(FileGuard&& other) noexcept;
	FileGuard& operator=(FileGuard&& other) noexcept;

	std::fstream& getStream();
	bool isFileOpen() const;
};

// Manages game save/load operations
class GameSaveManager {
private:
	SaveConfiguration config;

	void ensureSaveDirectoryExists() const;
	
	// JSON parsing helpers
	std::string extractJsonValue(const std::string& json, const std::string& key) const;
	int extractJsonInt(const std::string& json, const std::string& key) const;
	std::string extractJsonObject(const std::string& json, const std::string& key) const;
	std::string extractJsonArray(const std::string& json, const std::string& key) const;

public:
	GameSaveManager();
	explicit GameSaveManager(const SaveConfiguration& configuration);

	// Save game state to file (throws std::runtime_error on failure)
	void saveGame(const Game& game, const std::string& saveName) const;

	// Load game state from file (throws std::runtime_error on failure)
	void loadGame(Game& game, const std::string& saveName) const;

	// List all available saves (throws std::runtime_error on failure)
	std::vector<std::string> listSaves() const;

	// Check if save exists
	bool saveExists(const std::string& saveName) const;

	// Delete save file (throws std::runtime_error on failure)
	void deleteSave(const std::string& saveName) const;
};

#endif // GAME_SAVE_MANAGER_H
