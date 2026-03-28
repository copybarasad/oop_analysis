#include "save_manager.h"
#include "game.h"
#include "exceptions.h"
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cerrno>

#if defined(_WIN32)
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

static bool make_dirs_recursive(const std::string& path, std::string& err) {
    if (path.empty()) return true;
    std::string tmp = path;
    for (auto &c : tmp) if (c == '\\') c = '/';
    size_t start = 0;
    while (start < tmp.size()) {
        size_t slash = tmp.find('/', start);
        if (slash == std::string::npos) slash = tmp.size();
        std::string sub = tmp.substr(0, slash);
        if (!sub.empty()) {
#ifdef _WIN32
            if (_mkdir(sub.c_str()) != 0) {
                if (errno != EEXIST) { err = std::string("mkdir failed: ") + strerror(errno); return false; }
            }
#else
            if (mkdir(sub.c_str(), 0755) != 0) {
                if (errno != EEXIST) { err = std::string("mkdir failed: ") + strerror(errno); return false; }
            }
#endif
        }
        start = slash + 1;
    }
    return true;
}

void SaveManager::saveGameToFile(const Game& game, const std::string& filename) {
    
    std::string outPath = filename;
    std::string parent;
    size_t pos = outPath.find_last_of("/\\");
    if (pos != std::string::npos) parent = outPath.substr(0, pos);

    std::string err;
    if (!parent.empty()) {
        if (!make_dirs_recursive(parent, err)) {
            throw FileIOException(std::string("Cannot create directory '") + parent + "': " + err);
        }
    }

    std::string tmp = outPath + ".tmp";
    std::ofstream ofs;
    ofs.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    try {
        ofs.open(tmp, std::ios::trunc);
        
        game.saveToStream(ofs);
        ofs.close();
    } catch (const std::exception& ex) {
        
        std::remove(tmp.c_str());
        throw SaveException(std::string("Failed to write temporary save file '") + tmp + "': " + ex.what());
    }

    
    {
        std::ifstream check(outPath);
        if (check.is_open()) check.close();
        if (std::remove(outPath.c_str()) != 0) {
            if (errno != ENOENT) {
                std::remove(tmp.c_str());
                throw FileIOException(std::string("Cannot remove existing file '") + outPath + "': " + strerror(errno));
            }
        }
    }

    if (std::rename(tmp.c_str(), outPath.c_str()) != 0) {
        std::remove(tmp.c_str());
        throw FileIOException(std::string("Cannot rename temporary file '") + tmp + " to '" + outPath + "': " + strerror(errno));
    }
}

void SaveManager::loadGameFromFile(Game& game, const std::string& filename) {
    std::ifstream ifs;
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        ifs.open(filename);
    } catch (const std::exception& ex) {
        throw FileIOException(std::string("Cannot open save file '") + filename + "': " + ex.what());
    }

    try {
        game.loadFromStream(ifs);
    } catch (const std::exception& ex) {
        throw LoadException(std::string("Failed to parse or load save file '") + filename + "': " + ex.what());
    }
    return;
}