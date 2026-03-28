#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <memory>

#include "Cell.h"
#include "struct/savedata.h"
#include "Spell.h"
class GameManager;




enum class Mode {
    Read,
    Write
};


class FileStorage {

private:
    std::fstream file;
    std::string path;
    Mode mode;

public:
    // Конструктор (открывает файл)
    FileStorage(const std::string& filePath, Mode mode);

    // Деструктор (файл закрывается автоматически)
    ~FileStorage();

    // Сохранение структуры T в бинарный файл
    void Save(savedata data);

    // Загрузка структуры T из бинарного файла
    savedata Load();
};
