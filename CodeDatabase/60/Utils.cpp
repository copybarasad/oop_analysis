#include <Utils.hpp>
#include <stdexcept>
#include <RainOfArrows.hpp>
#include <SiegeArrow.hpp>
#include <PlayerTrap.hpp>
#include <SummonDecoySpell.hpp>
#include <EmpowerSpell.hpp>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

void Utils::printMultiline(WINDOW *win, int start_y, int start_x, const std::string& text) {
    std::stringstream ss(text);
    std::string line;
    int current_y = start_y;

    while (std::getline(ss, line, '\n')) {

        mvwprintw(win, current_y, start_x, "%s", line.c_str());
        current_y++; 
    }
}

void Utils::printMultiline(int start_y, int start_x, const std::string& text) {
    std::stringstream ss(text);
    std::string line;
    int current_y = start_y;

    while (std::getline(ss, line, '\n')) {

        mvprintw(current_y, start_x, "%s", line.c_str());
        current_y++; 
    }
}

std::string Utils::readFileToString(const std::string& filename) {
    std::ifstream fileStream(filename);
    if (!fileStream.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::stringstream buffer; 
    buffer << fileStream.rdbuf();
    return buffer.str();
}

void Utils::printNumbers(int y, int x, int number) {
    std::string sNumber = std::to_string(number);
    int i = x;
    for (auto ch : sNumber) {
        printMultiline(y, i, readFileToString("../assets/numbers/" + std::string(1, ch) + ".txt"));
        if (ch == '1') {
            i = i + 3;
        } else {
            i = i + 7;
        }
    }
}

    enum class Type {
        ENTITY,
        FLOOR,
        WALL,
        PLAYER,
        ENEMY,
        ENEMY_BASE,
        SPIKED_TRAP,
        PLAYER_TRAP,
        ARCHER_TOWER,
        DECOY_ALLY,
    };

std::string Utils::entityTypeToString(Entity::Type type) {
    switch (type) {
        case Entity::Type::PLAYER: return "PLAYER";
        case Entity::Type::ENEMY: return "ENEMY";
        case Entity::Type::ARCHER_TOWER: return "ARCHER_TOWER";
        case Entity::Type::ENEMY_BASE: return "ENEMY_BASE";
        case Entity::Type::SPIKED_TRAP: return "SPIKED_TRAP";
        case Entity::Type::PLAYER_TRAP: return "PLAYER_TRAP";
        case Entity::Type::DECOY_ALLY: return "DECOY_ALLY";
        default: return "UNKNOWN";
    }
}

std::string Utils::spellTypeToString(SpellType type) {
    switch (type) {
        case SpellType::SiegeArrow:   return "SiegeArrow";
        case SpellType::RainOfArrows: return "RainOfArrows";
        case SpellType::PlaceTrap:    return "PlaceTrap";
        case SpellType::SummonDecoy:  return "SummonDecoy";
        case SpellType::Empower:      return "Empower";
        default:                      return "Unknown";
    }
}

std::shared_ptr<Spell> Utils::spellFromString(const std::string& spellName) {
    if (spellName == "SiegeArrow")   return std::make_shared<SiegeArrow>();
    if (spellName == "RainOfArrows") return std::make_shared<RainOfArrows>();
    if (spellName == "PlaceTrap")    return std::make_shared<PlaceTrapSpell>();
    if (spellName == "SummonDecoy")  return std::make_shared<SummonDecoySpell>();
    if (spellName == "Empower")      return std::make_shared<EmpowerSpell>();
    return nullptr;
}

std::string Utils::getValueFromLine(const std::string& line) {
    size_t pos = line.find("=");
    if (pos == std::string::npos) return "";
    return line.substr(pos + 1);
}

Entity::Type Utils::entityTypeFromString(const std::string& type_str) {
    if (type_str == "PLAYER")        return Entity::Type::PLAYER;
    if (type_str == "ENEMY")         return Entity::Type::ENEMY;
    if (type_str == "ARCHER_TOWER")  return Entity::Type::ARCHER_TOWER;
    if (type_str == "ENEMY_BASE")    return Entity::Type::ENEMY_BASE;
    if (type_str == "DECOY_ALLY")    return Entity::Type::DECOY_ALLY;
    if (type_str == "PLAYER_TRAP")   return Entity::Type::PLAYER_TRAP;
    if (type_str == "SPIKED_TRAP")   return Entity::Type::SPIKED_TRAP;

    return Entity::Type::ENTITY; 
}

std::vector<Save> Utils::createSaves() {
    std::string pathToSaves = "./saves";
    std::vector<Save> saves; 

    if (!fs::exists(pathToSaves) || !fs::is_directory(pathToSaves)) {
        return saves; 
    }

    try {
        for (const auto& entry : fs::directory_iterator(pathToSaves)) {
            if (entry.is_regular_file()) {
                std::string fullname = entry.path().filename().string();
                std::string displayName = fullname; 

                size_t last_underscore = displayName.rfind('_');
                size_t last_dot = displayName.rfind('.');

                if (last_underscore != std::string::npos && 
                   (last_dot == std::string::npos || last_underscore < last_dot)) 
                {
                    displayName = displayName.substr(0, last_underscore);
                }

                saves.push_back(Save{
                    fullname, 
                    displayName, 
                    getLastModifiedTimeString(entry.path()), 
                    getPlayerLevelFromSave(entry.path().string()),
                    getFieldLevelFromSave(entry.path().string())
                }); 
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return saves; 
    }

    std::sort(saves.begin(), saves.end(), [](const Save& a, const Save& b) {

        return Utils::getIndexFromName(a.fullname) < Utils::getIndexFromName(b.fullname);
    });

    return saves; 
}

std::string Utils::getLastModifiedTimeString(const fs::path& filepath) {
    try {

        auto ftime = fs::last_write_time(filepath);

        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
        );

        std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&cftime), "%d.%m.%y %H:%M");
        return ss.str();
    } catch (const fs::filesystem_error& e) {

        return "N/A";
    }
}

int Utils::getPlayerLevelFromSave(const std::string& filepath) {
    std::ifstream saveFile(filepath);
    if (!saveFile.is_open()) {
        return -1; 
    }

    std::string line;
    bool inPlayerSection = false;

    while (std::getline(saveFile, line)) {

        if (line == "[Player]") {
            inPlayerSection = true;
            continue; 
        }

        if (inPlayerSection && line.rfind('[', 0) == 0) {
            inPlayerSection = false;
            break; 
        }

        if (inPlayerSection && line.rfind("level=", 0) == 0) {
            try {

                std::string levelString = Utils::getValueFromLine(line);
                if (!levelString.empty()) {

                    return std::stoi(levelString);
                }
            } catch (const std::exception& e) {

                return -1; 
            }
        }
    }

    return -1; 
}

bool Utils::deleteSaveFile(const std::string& filename) {

    if (filename.empty() || filename.find('/') != std::string::npos || filename.find('\\') != std::string::npos) {
        std::cerr << "Error: Invalid filename format for deletion: " << filename << std::endl;
        return false;
    }

    try {

        fs::path save_path = "saves";
        save_path /= filename;

        if (!fs::exists(save_path)) {

            return true;
        }

        if (!fs::is_regular_file(save_path)) {
            std::cerr << "Error: Path is not a regular file: " << save_path << std::endl;
            return false;
        }

        return fs::remove(save_path);

    } catch (const fs::filesystem_error& e) {

        std::cerr << "Filesystem error during deletion: " << e.what() << std::endl;
        return false;
    }
}

int Utils::getIndexFromName(const std::string& filename) {

    size_t last_underscore = filename.rfind('_');

    size_t last_dot = filename.rfind('.');

    if (last_underscore != std::string::npos && 
        (last_dot == std::string::npos || last_underscore < last_dot)) 
    {

        std::string index_str = filename.substr(last_underscore + 1);

        if (last_dot != std::string::npos && last_dot > last_underscore) {
            index_str = index_str.substr(0, last_dot - (last_underscore + 1));
        }

        try {

            return std::stoi(index_str);
        } catch (const std::exception& e) {

            return -1;
        }
    }

    return -1;
}

int Utils::getFieldLevelFromSave(const std::string& filepath) {
    std::ifstream saveFile(filepath);
    if (!saveFile.is_open()) {
        return -1; 
    }

    std::string line;
    while (std::getline(saveFile, line)) {
        if (line == "[Field]") {
            if (std::getline(saveFile, line) && line.rfind("turnCounter=", 0) == 0) {
                 if (std::getline(saveFile, line) && line.rfind("currentLevel=", 0) == 0) {
                    try {
                        std::string levelString = Utils::getValueFromLine(line);
                        if (!levelString.empty()) {
                            return std::stoi(levelString);
                        }
                    } catch (const std::exception& e) {
                        return -1; 
                    }
                 }
            }
            break; 
        }
    }
    return 1; 
}