#include "SaveManager.h"

void SaveManager::writePair(std::ofstream& out, const std::string& key, const std::string& value)
{
    out << key << "\n" << value << "\n";

    // Проверяем состояние потока: если что-то пошло не так кидаем исключение.
    if (!out.good()) {
        throw SaveWriteException("error while writing key '" + key + "'");
    }
}

// Чтение пары "ключ/значение" из файла.
// Возвращает false, если достигнут конец файла до чтения ключа.
// Если ключ прочитан, но нет строки значения — бросает SaveReadException.
bool SaveManager::readPair(std::ifstream& in, std::string& key, std::string& value){
        if (!std::getline(in, key)) {
            return false;
        }

        if (!std::getline(in, value)) {
            throw SaveReadException("unexpected end of file after key '" + key + "'");
        }

        return true;
}