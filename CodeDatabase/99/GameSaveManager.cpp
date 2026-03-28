#include "../h/GameSaveManager.h"
#include "../h/GameExceptions.h"
#include "../h/Game.h"
#include <filesystem>
#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;

// FileGuard implementation
FileGuard::FileGuard(const string& filename, ios::openmode mode) 
	: filename(filename), isOpen(false) {
	file.open(filename, mode);
	if (!file.is_open()) {
		throw SaveLoadException("open", filename, "File could not be opened");
	}
	isOpen = true;
}

FileGuard::~FileGuard() {
	if (isOpen && file.is_open()) {
		file.close();
	}
}

FileGuard::FileGuard(FileGuard&& other) noexcept 
	: file(std::move(other.file)), filename(std::move(other.filename)), isOpen(other.isOpen) {
	other.isOpen = false;
}

FileGuard& FileGuard::operator=(FileGuard&& other) noexcept {
	if (this != &other) {
		if (isOpen && file.is_open()) {
			file.close();
		}
		file = std::move(other.file);
		filename = std::move(other.filename);
		isOpen = other.isOpen;
		other.isOpen = false;
	}
	return *this;
}

fstream& FileGuard::getStream() {
	if (!isOpen) {
		throw SaveLoadException("access", filename, "File is not open");
	}
	return file;
}

bool FileGuard::isFileOpen() const {
	return isOpen && file.is_open();
}

// GameSaveManager implementation
GameSaveManager::GameSaveManager() : config() {
}

GameSaveManager::GameSaveManager(const SaveConfiguration& configuration) : config(configuration) {
}

void GameSaveManager::ensureSaveDirectoryExists() const {
	try {
		const string& directory = config.getSaveDirectory();
		if (!filesystem::exists(directory)) {
			filesystem::create_directories(directory);
		}
	} catch (const filesystem::filesystem_error& e) {
		throw SaveLoadException("create directory", config.getSaveDirectory(), e.what());
	}
}

void GameSaveManager::saveGame(const Game& game, const string& saveName) const {
	try {
		ensureSaveDirectoryExists();
		
		string filepath = config.buildFilePath(saveName);
		FileGuard fileGuard(filepath, ios::out);
		auto& file = fileGuard.getStream();

		// Get game data
		const Player& player = game.getPlayer();
		const Field& field = game.getField();
		const auto& enemies = game.getEnemies();
		Position playerPos = field.getUnitPosition(&player);

		// Create JSON structure
		file << "{\n";
		file << "  \"version\": " << config.getSaveVersion() << ",\n";
		file << "  \"gameState\": " << static_cast<int>(game.getState()) << ",\n";
		file << "  \"currentLevel\": " << game.getCurrentLevel() << ",\n";
		
		// Player data
		file << "  \"player\": {\n";
		file << "    \"hp\": " << player.getHP() << ",\n";
		file << "    \"damage\": " << player.getDamage() << ",\n";
		file << "    \"score\": " << player.getScore() << ",\n";
		file << "    \"position\": {\n";
		file << "      \"x\": " << playerPos.x << ",\n";
		file << "      \"y\": " << playerPos.y << "\n";
		file << "    }\n";
		file << "  },\n";
		
		// Field data
		file << "  \"field\": {\n";
		file << "    \"width\": " << field.getWidth() << ",\n";
		file << "    \"height\": " << field.getHeight() << "\n";
		file << "  },\n";
		
		// Enemies data
		file << "  \"enemies\": [\n";
		for (size_t i = 0; i < enemies.size(); ++i) {
			const auto& enemy = enemies[i];
			Position enemyPos = field.getUnitPosition(enemy.get());
			
			file << "    {\n";
			file << "      \"hp\": " << enemy->getHP() << ",\n";
			file << "      \"damage\": " << enemy->getDamage() << ",\n";
			file << "      \"position\": {\n";
			file << "        \"x\": " << enemyPos.x << ",\n";
			file << "        \"y\": " << enemyPos.y << "\n";
			file << "      }\n";
			file << "    }";
			if (i < enemies.size() - 1) {
				file << ",";
			}
			file << "\n";
		}
		file << "  ]\n";
		file << "}\n";

		if (!file.good()) {
			throw SaveLoadException("write", filepath, "Write operation failed");
		}

	} catch (const SaveLoadException&) {
		throw; // Re-throw save/load exceptions
	} catch (const exception& e) {
		throw SaveLoadException("save", saveName, e.what());
	}
}

void GameSaveManager::loadGame(Game& game, const string& saveName) const {
	string filepath = config.buildFilePath(saveName);
	
	if (!saveExists(saveName)) {
		throw SaveLoadException("load", filepath, "Save file does not exist");
	}

	try {
		FileGuard fileGuard(filepath, ios::in);
		auto& file = fileGuard.getStream();

		// Read entire JSON file
		stringstream buffer;
		buffer << file.rdbuf();
		string jsonContent = buffer.str();
		
		if (jsonContent.empty()) {
			throw CorruptedSaveException(filepath, "Empty save file");
		}

		// Parse and verify save version
		int version = extractJsonInt(jsonContent, "version");
		if (version < 1 || version > config.getSaveVersion()) {
			throw CorruptedSaveException(filepath, "Incompatible save version: " + to_string(version) + ". Expected version 1-" + to_string(config.getSaveVersion()));
		}

		// Parse game state
		int gameStateInt = extractJsonInt(jsonContent, "gameState");
		GameState state = static_cast<GameState>(gameStateInt);

		// Parse current level
		int currentLevel = extractJsonInt(jsonContent, "currentLevel");

		// Parse player data
		string playerJson = extractJsonObject(jsonContent, "player");
		int playerHP = extractJsonInt(playerJson, "hp");
		int playerDamage = extractJsonInt(playerJson, "damage");
		int playerScore = extractJsonInt(playerJson, "score");
		
		string playerPosJson = extractJsonObject(playerJson, "position");
		int playerX = extractJsonInt(playerPosJson, "x");
		int playerY = extractJsonInt(playerPosJson, "y");
		Position playerPos = {playerX, playerY};

		// Validate loaded data
		if (playerHP < 0 || playerDamage < 0 || playerScore < 0) {
			throw CorruptedSaveException(filepath, "Invalid player stats");
		}

		// Parse field data (for validation only)
		string fieldJson = extractJsonObject(jsonContent, "field");
		extractJsonInt(fieldJson, "width");  // Validate field exists
		extractJsonInt(fieldJson, "height"); // Validate field exists

		// Clear current game state
		game.clearEnemies();

		// Restore game state
		game.restoreGameState(state, currentLevel);

		// Restore player
		game.restorePlayer(playerHP, playerDamage, playerScore, playerPos);

		// Parse and restore enemies
		string enemiesArrayJson = extractJsonArray(jsonContent, "enemies");
		
		// Simple enemy parsing - split by objects
		size_t pos = 0;
		while (pos < enemiesArrayJson.length()) {
			// Find next enemy object
			size_t objStart = enemiesArrayJson.find('{', pos);
			if (objStart == string::npos) break;
			
			size_t objEnd = objStart + 1;
			int braceCount = 1;
			while (objEnd < enemiesArrayJson.length() && braceCount > 0) {
				if (enemiesArrayJson[objEnd] == '{') braceCount++;
				else if (enemiesArrayJson[objEnd] == '}') braceCount--;
				objEnd++;
			}
			
			string enemyJson = enemiesArrayJson.substr(objStart, objEnd - objStart);
			
			// Parse enemy data
			int enemyHP = extractJsonInt(enemyJson, "hp");
			int enemyDamage = extractJsonInt(enemyJson, "damage");
			
			string enemyPosJson = extractJsonObject(enemyJson, "position");
			int enemyX = extractJsonInt(enemyPosJson, "x");
			int enemyY = extractJsonInt(enemyPosJson, "y");
			Position enemyPos = {enemyX, enemyY};

			// Validate enemy data
			if (enemyHP < 0 || enemyDamage < 0) {
				throw CorruptedSaveException(filepath, "Invalid enemy stats");
			}

			game.restoreEnemy(enemyHP, enemyDamage, enemyPos);
			
			pos = objEnd;
		}

	} catch (const SaveLoadException&) {
		throw; // Re-throw save/load exceptions
	} catch (const CorruptedSaveException&) {
		throw; // Re-throw corruption exceptions
	} catch (const exception& e) {
		throw SaveLoadException("load", filepath, e.what());
	}
}

vector<string> GameSaveManager::listSaves() const {
	vector<string> saves;
	
	try {
		const string& directory = config.getSaveDirectory();
		if (!filesystem::exists(directory)) {
			return saves; // Return empty list if directory doesn't exist
		}

		for (const auto& entry : filesystem::directory_iterator(directory)) {
			if (entry.is_regular_file()) {
				string filename = entry.path().filename().string();
				const string& extension = config.getSaveExtension();
				if (filename.ends_with(extension)) {
					// Remove extension to get save name
					string saveName = filename.substr(0, filename.length() - extension.length());
					saves.push_back(saveName);
				}
			}
		}
	} catch (const filesystem::filesystem_error& e) {
		throw SaveLoadException("list saves", config.getSaveDirectory(), e.what());
	}

	return saves;
}

bool GameSaveManager::saveExists(const string& saveName) const {
	string filepath = config.buildFilePath(saveName);
	return filesystem::exists(filepath);
}

void GameSaveManager::deleteSave(const string& saveName) const {
	string filepath = config.buildFilePath(saveName);
	
	if (!saveExists(saveName)) {
		throw SaveLoadException("delete", filepath, "Save file does not exist");
	}
	
	try {
		if (!filesystem::remove(filepath)) {
			throw SaveLoadException("delete", filepath, "Failed to delete file");
		}
	} catch (const filesystem::filesystem_error& e) {
		throw SaveLoadException("delete", filepath, e.what());
	}
}

// Simple JSON parsing helpers
string GameSaveManager::extractJsonValue(const string& json, const string& key) const {
	string searchKey = "\"" + key + "\":";
	size_t pos = json.find(searchKey);
	if (pos == string::npos) {
		throw CorruptedSaveException("JSON", "Key '" + key + "' not found");
	}
	
	pos += searchKey.length();
	while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
	
	size_t start = pos;
	size_t end = pos;
	
	if (json[pos] == '"') {
		// String value
		start = ++pos;
		while (end < json.length() && json[end] != '"') end++;
	} else if (json[pos] == '{') {
		// Object value
		int braceCount = 1;
		end = ++pos;
		while (end < json.length() && braceCount > 0) {
			if (json[end] == '{') braceCount++;
			else if (json[end] == '}') braceCount--;
			end++;
		}
		return json.substr(start, end - start);
	} else if (json[pos] == '[') {
		// Array value
		int bracketCount = 1;
		end = ++pos;
		while (end < json.length() && bracketCount > 0) {
			if (json[end] == '[') bracketCount++;
			else if (json[end] == ']') bracketCount--;
			end++;
		}
		return json.substr(start, end - start);
	} else {
		// Number value
		while (end < json.length() && json[end] != ',' && json[end] != '}' && json[end] != '\n') end++;
	}
	
	return json.substr(start, end - start);
}

int GameSaveManager::extractJsonInt(const string& json, const string& key) const {
	string value = extractJsonValue(json, key);
	try {
		return stoi(value);
	} catch (const exception&) {
		throw CorruptedSaveException("JSON", "Invalid integer value for key '" + key + "': " + value);
	}
}

string GameSaveManager::extractJsonObject(const string& json, const string& key) const {
	string searchKey = "\"" + key + "\":";
	size_t pos = json.find(searchKey);
	if (pos == string::npos) {
		throw CorruptedSaveException("JSON", "Object key '" + key + "' not found");
	}
	
	pos += searchKey.length();
	while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
	
	if (json[pos] != '{') {
		throw CorruptedSaveException("JSON", "Expected object for key '" + key + "'");
	}
	
	size_t start = pos;
	int braceCount = 1;
	size_t end = ++pos;
	
	while (end < json.length() && braceCount > 0) {
		if (json[end] == '{') braceCount++;
		else if (json[end] == '}') braceCount--;
		end++;
	}
	
	return json.substr(start, end - start);
}

string GameSaveManager::extractJsonArray(const string& json, const string& key) const {
	string searchKey = "\"" + key + "\":";
	size_t pos = json.find(searchKey);
	if (pos == string::npos) {
		throw CorruptedSaveException("JSON", "Array key '" + key + "' not found");
	}
	
	pos += searchKey.length();
	while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
	
	if (json[pos] != '[') {
		throw CorruptedSaveException("JSON", "Expected array for key '" + key + "'");
	}
	
	size_t start = pos + 1; // Skip opening bracket
	int bracketCount = 1;
	size_t end = ++pos;
	
	while (end < json.length() && bracketCount > 0) {
		if (json[end] == '[') bracketCount++;
		else if (json[end] == ']') bracketCount--;
		end++;
	}
	
	return json.substr(start, end - start - 1); // Skip closing bracket
}

