#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <string>

class Game;

class SaveManager {
private:
	static const std::string SAVE_FILE;
	
	void writeHeader(std::ofstream& file, int level, int turn, int score, int score_threshold);
	void writeFieldInfo(std::ofstream& file, int width, int height);
	void writePlayerInfo(std::ofstream& file, Game& game);
	void writeEntitiesInfo(std::ofstream& file, Game& game);
	
	void readAndValidateHeader(std::ifstream& file, int& level, int& turn, int& score, int& score_threshold);
	void readFieldInfo(std::ifstream& file, int& width, int& height);
	void readPlayerInfo(std::ifstream& file, Game& game);
	void readEntitiesInfo(std::ifstream& file, Game& game);

public:
	SaveManager() = default;
	
	void saveGame(Game& game);
	void loadGame(Game& game);
	
	bool saveFileExists() const;
};

#endif
