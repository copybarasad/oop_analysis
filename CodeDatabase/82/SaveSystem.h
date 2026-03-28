#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <string>
#include <vector>
#include <memory>

class GameState;

class SaveSystem {
public:
    static void saveGame(const GameState& gameState, const std::string& filename);
    static std::unique_ptr<GameState> loadGame(const std::string& filename);
private:
    static void writeValue(std::ofstream& file, const std::string& key, int value);
    static void writeValue(std::ofstream& file, const std::string& key, const std::string& value);
    static void writePosition(std::ofstream& file, const std::string& key, int x, int y);

    static int readIntValue(const std::vector<std::string>& lines, const std::string& key);
    static std::string readStringValue(const std::vector<std::string>& lines, const std::string& key);
    static std::pair<int, int> readPositionValue(const std::vector<std::string>& lines, const std::string& key);

    static std::vector<std::string> readLines(const std::string& filename);
    static std::vector<std::string> split(const std::string& str, char delimiter);
};

#endif 