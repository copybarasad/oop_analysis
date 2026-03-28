#include "save_manager.h"
#include "game.h"
#include "game_exceptions.h"
#include <fstream>

namespace rpg {

FileGuard::FileGuard(const std::string& filename, std::ios::openmode mode)
    : file_(filename, mode), filename_(filename) {
    if (!file_.is_open()) {
        throw FileNotFoundException(filename);
    }
}

FileGuard::~FileGuard() {
    if (file_.is_open()) {
        file_.close();
    }
}

std::fstream& FileGuard::GetStream() {
    return file_;
}

bool FileGuard::IsOpen() const {
    return file_.is_open();
}


void SaveManager::SaveGame(const Game& game, const std::string& filename) {
    try {
        FileGuard file_guard(filename, std::ios::out | std::ios::binary);
        std::fstream& file = file_guard.GetStream();

        GameMemento memento = game.CreateMemento(); // Game создает свой memento
        serializer_.Serialize(memento, file); // Serializer сериализует memento

        if (file.fail()) {
            throw FileSaveException(filename, "ошибка записи данных");
        }

    } catch (const FileNotFoundException&) {
        throw FileSaveException(filename, "не удается создать файл");
    }
}

void SaveManager::LoadGame(Game& game, const std::string& filename) {
    FileGuard file_guard(filename, std::ios::in | std::ios::binary);
    std::fstream& file = file_guard.GetStream();

    GameMemento memento = serializer_.Deserialize(file);

    if (file.fail()) {
        throw FileCorruptedException(filename, "неожиданная ошибка чтения");
    }

    game.RestoreFromMemento(memento);
}

bool SaveManager::SaveFileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

}
