#pragma once
#include <string>
#include <vector>

class Game;


class SaveManager {
public:
    // Сохранить игру
    static void Save(const Game& game, const std::string& filename);

    // Загрузить игру
    static void Load(Game& game, const std::string& filename);

private:
    // Вспомогательные функции парсинга
    static std::string ReadFile(const std::string& filename);
    static std::string Escape(const std::string& s);

    static int ParseInt(const std::string& src, const std::string& key);
    static std::string ParseString(const std::string& src, const std::string& key);
    static std::vector<std::string> ParseStringArray(const std::string& src, const std::string& key);

    // Загрузка клеток поля
    static void LoadField(const std::string& src, Game& game);
};