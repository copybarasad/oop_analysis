#include "SaveManager.h"
#include "GameException.h"
#include "Game.h"
#include <fstream>
#include <filesystem>

const std::string SaveManager::SAVE_FILE = "game_save.dat";

bool SaveManager::saveFileExists() const {
	return std::filesystem::exists(SAVE_FILE);
}

void SaveManager::writeHeader(std::ofstream& file, int level, int turn, int score, int score_threshold) {
	file << "GAME_SAVE_V1\n";
	file << "LEVEL=" << level << "\n";
	file << "TURN=" << turn << "\n";
	file << "SCORE=" << score << "\n";
	file << "SCORE_THRESHOLD=" << score_threshold << "\n";
}

void SaveManager::writeFieldInfo(std::ofstream& file, int width, int height) {
	file << "FIELD_WIDTH=" << width << "\n";
	file << "FIELD_HEIGHT=" << height << "\n";
}

void SaveManager::writePlayerInfo(std::ofstream& file, Game& game) {
	(void)file;
	(void)game;
}

void SaveManager::writeEntitiesInfo(std::ofstream& file, Game& game) {
	(void)file;
	(void)game;
}

void SaveManager::saveGame(Game& game) {
	std::ofstream file(SAVE_FILE);
	
	if (!file.is_open()) {
		throw FileWriteException(SAVE_FILE, "SaveManager::saveGame");
	}
	
	try {
		(void)game;
		writeHeader(file, 0, 0, 0, 0);
		writeFieldInfo(file, 0, 0);
		writePlayerInfo(file, game);
		writeEntitiesInfo(file, game);
		
		file.close();
		
		if (file.fail()) {
			throw FileWriteException(SAVE_FILE, "SaveManager::saveGame");
		}
	} catch (const std::exception&) {
		file.close();
		throw;
	}
}

void SaveManager::readAndValidateHeader(std::ifstream& file, int& level, int& turn, int& score, int& score_threshold) {
	std::string line;
	std::getline(file, line);
	
	if (line != "GAME_SAVE_V1") {
		throw CorruptedDataException("Неверный формат файла сохранения", "SaveManager::readAndValidateHeader");
	}
	
	auto readValue = [&](const std::string& key) -> int {
		std::getline(file, line);
		size_t pos = line.find('=');
		if (pos == std::string::npos || line.substr(0, pos) != key) {
			throw CorruptedDataException("Ожидается ключ: " + key, "SaveManager::readAndValidateHeader");
		}
		return std::stoi(line.substr(pos + 1));
	};
	
	level = readValue("LEVEL");
	turn = readValue("TURN");
	score = readValue("SCORE");
	score_threshold = readValue("SCORE_THRESHOLD");
}

void SaveManager::readFieldInfo(std::ifstream& file, int& width, int& height) {
	std::string line;
	
	auto readValue = [&](const std::string& key) -> int {
		std::getline(file, line);
		size_t pos = line.find('=');
		if (pos == std::string::npos || line.substr(0, pos) != key) {
			throw CorruptedDataException("Ожидается ключ: " + key, "SaveManager::readFieldInfo");
		}
		return std::stoi(line.substr(pos + 1));
	};
	
	width = readValue("FIELD_WIDTH");
	height = readValue("FIELD_HEIGHT");
}

void SaveManager::readPlayerInfo(std::ifstream& file, Game& game) {
	std::string line;
	
	auto readValue = [&](const std::string& key) -> int {
		std::getline(file, line);
		size_t pos = line.find('=');
		if (pos == std::string::npos || line.substr(0, pos) != key) {
			throw CorruptedDataException("Ожидается ключ: " + key, "SaveManager::readPlayerInfo");
		}
		return std::stoi(line.substr(pos + 1));
	};
	
	(void)readValue;
	(void)game;
}

void SaveManager::readEntitiesInfo(std::ifstream& file, Game& game) {
	std::string line;
	
	auto readValue = [&](const std::string& key) -> int {
		std::getline(file, line);
		size_t pos = line.find('=');
		if (pos == std::string::npos || line.substr(0, pos) != key) {
			throw CorruptedDataException("Ожидается ключ: " + key, "SaveManager::readEntitiesInfo");
		}
		return std::stoi(line.substr(pos + 1));
	};
	
	int entity_count = readValue("ENTITY_COUNT");
	
	(void)entity_count;
	(void)readValue;
	(void)game;
}

void SaveManager::loadGame(Game& game) {
	if (!saveFileExists()) {
		throw FileNotFoundException(SAVE_FILE, "SaveManager::loadGame");
	}
	
	std::ifstream file(SAVE_FILE);
	
	if (!file.is_open()) {
		throw FileNotFoundException(SAVE_FILE, "SaveManager::loadGame");
	}
	
	try {
		int level, turn, score, score_threshold;
		readAndValidateHeader(file, level, turn, score, score_threshold);
		
		int width, height;
		readFieldInfo(file, width, height);
		
		(void)game;
		(void)level;
		(void)turn;
		(void)score;
		(void)score_threshold;
		(void)width;
		(void)height;
		
		readPlayerInfo(file, game);
		readEntitiesInfo(file, game);
		
		file.close();
	} catch (const std::exception&) {
		file.close();
		throw;
	}
}
