#include "SaveManager.h"
#include "Game.h"
#include "GameField.h"
#include "Player.h"
#include "Entity.h"
#include "Enemy.h"
#include "Ally.h"
#include "Enhancement.h"
#include "Cell.h"
#include "SpellFactory.h"
#include "SaveExceptions.h"


#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

// Вспомогательные функции

std::string SaveManager::Escape(const std::string& s) {
    std::string out;
    for (char c : s) out += (c == '"' ? "\\\"" : std::string(1, c));
    return out;
}

std::string SaveManager::ReadFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open())
    throw FileOpenException("Не удалось открыть файл: " + filename);

    std::stringstream buffer;
    buffer << in.rdbuf();
    return buffer.str();
}

int SaveManager::ParseInt(const std::string& src, const std::string& key) {
    auto pos = src.find(key);
    if (pos == std::string::npos) {
        std::cerr << "\n[DEBUG] ParseInt: не найден ключ '" << key << "' в строке:\n";
        std::cerr << src << "\n\n";
        throw SaveFormatException("Не найдено поле: " + key);
    }

    pos = src.find(":", pos);
    if (pos == std::string::npos)
        throw SaveFormatException("Ошибка формата (нет ':'): " + key);

    std::stringstream ss;
    for (size_t i = pos + 1; i < src.size(); i++) {
        if (isdigit(src[i]) || src[i]=='-')
            ss << src[i];
        else if (!isspace((unsigned char)src[i]))
            break;
    }

    int v = 0;
    ss >> v;
    return v;
}


std::string SaveManager::ParseString(const std::string& src, const std::string& key) {
    size_t pos = src.find(key);
    if (pos == std::string::npos)
        throw SaveFormatException("Missing key: " + key);

    pos = src.find('"', pos);
    if (pos == std::string::npos)
        throw SaveFormatException("Malformed string: " + key);

    size_t end = src.find('"', pos + 1);
    if (end == std::string::npos)
        throw SaveFormatException("Unclosed string: " + key);

    return src.substr(pos + 1, end - pos - 1);
}

std::vector<std::string> SaveManager::ParseStringArray(
    const std::string& src,
    const std::string& key
) {
    std::vector<std::string> result;

    auto pos = src.find(key);
    pos = src.find("[", pos);
    size_t end = src.find("]", pos);

    std::string arr = src.substr(pos + 1, end - pos - 1);

    std::stringstream ss(arr);
    std::string item;

    while (std::getline(ss, item, ',')) {
        auto q1 = item.find("\"");
        auto q2 = item.rfind("\"");
        if (q1 != std::string::npos && q2 != std::string::npos)
            result.push_back(item.substr(q1 + 1, q2 - q1 - 1));
    }

    return result;
}

// Сохранение игры

void SaveManager::Save(const Game& game, const std::string& filename) {
    std::filesystem::create_directories("saves");

    std::ofstream out(filename);
    if (!out.is_open())
    throw FileOpenException("Не удалось открыть файл: " + filename);

    const GameField& f = game.GetField();
    const Player& p = game.GetPlayer();

    auto [px, py] = f.PlayerCell();

    out << "{\n";
    out << " player: {\n";
    out << "   name: \"" << Escape(p.Name()) << "\",\n";
    out << "   hp: "  << p.Health() << ",\n";
    out << "   dmg: " << p.Damage() << ",\n";
    out << "   x: " << px << ",\n";
    out << "   y: " << py << ",\n";

    const Enhancement& e = p.Enh();
    out << "   enh: {"
        << " radius:" << e.radiusBonus
        << ", area:" << e.areaBonus
        << ", trap:" << e.trapDamageBonus
        << ", summon:" << e.summonBonus
        << ", stacks:" << e.stacks
        << " },\n";

    // рука
    out << "   hand: [";
    for (size_t i = 0; i < p.HandRef().Size(); i++) {
        out << "\"" << Escape(p.HandRef().Get(i)->Name()) << "\"";
        if (i + 1 < p.HandRef().Size()) out << ", ";
    }
    out << "]\n";

    out << " },\n";

    // поле 
    out << " field: {\n";
    out << "   width: " << f.Width() << ",\n";
    out << "   height: " << f.Height() << ",\n";
    out << "   cells: [\n";

    for (int y = 0; y < f.Height(); y++) {
        out << "     [";
        for (int x = 0; x < f.Width(); x++) {
            const Cell& c = f.CellAt(x, y);

            if (!c.isWalkable()) {
                out << "\"Wall\"";
            }
            else if (c.hasTrap()) {
                out << "{ trap:" << c.hasTrap() << " }";
            }
            else if (c.hasEntity()) {
                auto ent = c.getEntity();
                out << "{ type: \"" << ent->Name()
                    << "\", hp:" << ent->Health()
                    << ", dmg:" << ent->Damage()
                    << " }";
            }
            else {
                out << "\"Empty\"";
            }

            if (x + 1 < f.Width()) out << ", ";
        }
        out << "]";
        if (y + 1 < f.Height()) out << ",";
        out << "\n";
    }

    out << "   ]\n";
    out << " }\n";
    out << "}\n";
}

// Загрузка игры

void SaveManager::Load(Game& game, const std::string& filename){
    std::string src = ReadFile(filename);

    // Ищем "player"
    size_t ps = src.find("player");
    if (ps == std::string::npos)
        throw SaveFormatException("Не найден блок player");

    // Ищем первую '{' после "player"
    size_t braceStart = src.find("{", ps);
    if (braceStart == std::string::npos)
        throw SaveFormatException("Не найдена '{' после player");

    // Ищем соответствующую '}' по глубине скобок
    int depth = 0;
    size_t i = braceStart;
    for (; i < src.size(); ++i) {
        if (src[i] == '{')
            depth++;
        else if (src[i] == '}') {
            depth--;
            if (depth == 0) { // вышли из блока player
                ++i; // захватываем '}' в блок
                break;
            }
        }
    }
    if (depth != 0)
        throw SaveFormatException("Скобки блока player несбалансированы");

    
    std::string pblock = src.substr(braceStart, i - braceStart);

    std::string name = ParseString(pblock, "name");
    int hp  = ParseInt(pblock, "hp");
    int dmg = ParseInt(pblock, "dmg");
    int px  = ParseInt(pblock, "x");
    int py  = ParseInt(pblock, "y");

    Enhancement enh;
    enh.radiusBonus     = ParseInt(pblock, "radius");
    enh.areaBonus       = ParseInt(pblock, "area");
    enh.trapDamageBonus = ParseInt(pblock, "trap");
    enh.summonBonus     = ParseInt(pblock, "summon");
    enh.stacks          = ParseInt(pblock, "stacks");

    auto spells = ParseStringArray(pblock, "hand");

    // Создаём нового игрока 
    auto newPlayer = std::make_shared<Player>(
        name,
        hp,
        dmg,
        game.GetPlayer().HandRef().Capacity()
    );
    newPlayer->SetEnhancement(enh);

    for (auto& s : spells) {
        if (s=="DirectDamage") newPlayer->HandRef().Add(SpellFactory::MakeDirect());
        else if (s=="AreaDamage") newPlayer->HandRef().Add(SpellFactory::MakeArea());
        else if (s=="Trap")       newPlayer->HandRef().Add(SpellFactory::MakeTrap());
        else if (s=="Summon")     newPlayer->HandRef().Add(SpellFactory::MakeSummon());
        else if (s=="Enhance")    newPlayer->HandRef().Add(SpellFactory::MakeEnhance());
    }

    int w = ParseInt(src, "width");
    int h = ParseInt(src, "height");

    game.ResetField(w, h);
    GameField& field = game.GetField();
    // Загружаем поле
    LoadField(src, game);

    // Удаляем возможного "старого" игрока из клетки (если остался от сохранения)
    auto [ox, oy] = field.PlayerCell();
    if (ox != -1)
        field.CellAt(ox, oy).setEntity(nullptr);

    // Проверяем координаты игрока
    if (!field.Inside(px, py)) {
        std::cout << "Ошибка: позиция игрока вне поля, ставлю (0,0)\n";
        px = 0;
        py = 0;
    }

    // Ставим нового игрока и регистрируем в Game
    field.PlacePlayer(newPlayer, px, py);
    game.SetPlayer(newPlayer);
}



// Разбор клеток поля

void SaveManager::LoadField(const std::string& src, Game& game){
    GameField& field = game.GetField();
    int w = ParseInt(src, "width");
    int h = ParseInt(src, "height");

    // Находим начало массива cells
    auto pos = src.find("cells");
    pos = src.find("[", pos);
    if (pos == std::string::npos)
        throw SaveFormatException("Не найден массив cells");

    size_t globalPos = pos + 1;

    for (int y = 0; y < h; ++y) {

        size_t rowStart = src.find("[", globalPos);
        size_t rowEnd   = src.find("]", rowStart);
        if (rowStart == std::string::npos || rowEnd == std::string::npos)
            throw SaveFormatException("Ошибка разбора строки поля y=" + std::to_string(y));

        std::string row = src.substr(rowStart + 1, rowEnd - rowStart - 1);

        size_t i = 0;
        int x = 0;

        while (x < w && i < row.size()) {

            // пропускаем пробелы и запятые
            while (i < row.size() && (row[i] == ' ' || row[i] == '\t' || row[i] == ',')) {
                ++i;
            }
            if (i >= row.size()) break;

            if (row[i] == '"') {
                size_t q2 = row.find('"', i + 1);
                if (q2 == std::string::npos)
                    throw SaveFormatException("Некорректная строка в cells");

                std::string token = row.substr(i + 1, q2 - i - 1);

                if (token == "Wall")
                    field.CellAt(x, y).setWalkable(false);
                // "Empty" — ничего не делаем

                i = q2 + 1;
            }
            else if (row[i] == '{') {
                int depth = 0;
                size_t j = i;
                for (; j < row.size(); ++j) {
                    if (row[j] == '{')
                        ++depth;
                    else if (row[j] == '}') {
                        --depth;
                        if (depth == 0) { ++j; break; }
                    }
                }
                if (depth != 0)
                    throw SaveFormatException("Несбалансированные скобки в описании клетки");

                std::string obj = row.substr(i, j - i);
                // obj: { trap:5 } или { type: "Enemy", hp:..., dmg:... }

                if (obj.find("trap") != std::string::npos) {
                    int dmg = ParseInt(obj, "trap");
                    field.CellAt(x, y).placeTrap(dmg);
                }
                else if (obj.find("type") != std::string::npos) {
                    std::string t = ParseString(obj, "type");

                    if (t == "Hero") {
                        // Игрок восстанавливается отдельно, этот объект игнорируем
                    }
                    else if (t == "Enemy") {
                        int ehp  = ParseInt(obj, "hp");
                        int edmg = ParseInt(obj, "dmg");
                        field.CellAt(x, y).setEntity(
                            std::make_shared<Enemy>("Enemy", ehp, edmg)
                        );
                    }
                    else if (t == "Ally") {
                        int ahp  = ParseInt(obj, "hp");
                        int admg = ParseInt(obj, "dmg");
                        field.CellAt(x, y).setEntity(
                            std::make_shared<Ally>("Ally", ahp, admg)
                        );
                    }
                }

                i = j;
            }
            else {
                // неизвестный символ — просто сдвигаемся
                ++i;
                continue;
            }

            ++x;
        }

        globalPos = rowEnd + 1;
    }
}