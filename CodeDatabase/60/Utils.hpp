#pragma once
#include <string>
#include <ncurses.h>
#include <Entity.hpp>
#include <memory>
#include <SpellType.hpp>
#include <Spell.hpp>
#include <Save.hpp>
#include <vector>
#include <filesystem>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>

namespace fs = std::filesystem;

class Utils {
public:
    static void printMultiline(WINDOW *win, int start_y, int start_x, const std::string& text);
    static void printMultiline(int start_y, int start_x, const std::string& text);
    static std::string readFileToString(const std::string& filename);
    static void printNumbers(int y, int x, int number);
    static std::string entityTypeToString(Entity::Type type);
    static Entity::Type entityTypeFromString(const std::string& type_str);
    static std::string spellTypeToString(SpellType type);
    static std::shared_ptr<Spell> spellFromString(const std::string& spellName);
    static std::string getValueFromLine(const std::string& line);
    static std::vector<Save> createSaves();
    static bool deleteSaveFile(const std::string& filename);
    static int getFieldLevelFromSave(const std::string& filepath);
    
private:
    static int getIndexFromName(const std::string& filename);
    static std::string getLastModifiedTimeString(const fs::path& filepath);
    static int getPlayerLevelFromSave(const std::string& filepath);
};