#include "../include/core/save_manager.h"
#include "../include/core/game_board.h"
#include "../include/entity/player.h"
#include "../include/items/weapon.h"
#include "../include/entity/enemy_tower.h"
#include "../include/entity/enemy.h"
#include "../include/entity/friend_enemy.h"
#include "../include/entity/hand.h"
#include "../include/items/spell.h"
#include "../include/items/inventory.h"

#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include "../include/json.hpp"

using json = nlohmann::json;

static bool DirExists(const std::string& path) {
    struct stat st{};
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

static bool EnsureDir(const std::string& path) {
    if (DirExists(path)) return true;
#ifdef __APPLE__
    return mkdir(path.c_str(), 0755) == 0;
#else
    return mkdir(path.c_str()) == 0;
#endif
}

std::vector<std::string> SaveManager::ListSaves(const std::string& dir) {
    std::vector<std::string> names;
    if (!DirExists(dir)) {
        std::cerr << "Ошибка: Директория '" << dir << "' не существует." << std::endl;
        return names;
    }

    DIR* d = opendir(dir.c_str());
    if (!d) {
        std::cerr << "Ошибка: Не удалось открыть директорию '" << dir << "'." << std::endl;
        return names;
    }
    
    dirent* e;
    while ((e = readdir(d))) {
        std::string fn = e->d_name;
        if (fn.size() > 5 && fn.substr(fn.size() - 5) == ".json") {
            names.push_back(fn.substr(0, fn.size() - 5));
        }
    }

    closedir(d);

    if (names.empty()) {
        std::cout << "Сохранения не найдены в директории '" << dir << "'." << std::endl;
    }

    return names;
}

static std::string SpellTypeToStr(SpellType t) {
    switch (t) {
        case SpellType::Heal: return "Heal";
        case SpellType::Damage: return "Damage";
        case SpellType::Splash: return "Splash";
        case SpellType::Trap: return "Trap";
        case SpellType::Summon: return "Summon";
        case SpellType::Enhance: return "Enhance";
    }
    return "Heal";
}

static Spell* CreateSpellByName(const std::string& name) {
    if (name == "Heal") return new HealSpell(30, 50);
    if (name == "Damage") return new DamageSpell(40, 75);
    if (name == "Splash") return new SplashSpell(30, 3, 2, 100);
    if (name == "Trap") return new TrapSpell(50, 60);
    if (name == "Summon") return new SummonSpell(1, 80);
    if (name == "Enhance") return new EnhanceSpell(40);
    
        std::cerr << "Предупреждение: Неизвестный тип заклинания '" << name << "'." << std::endl;
    return nullptr;
}

bool SaveManager::Save(const std::string& dir, const std::string& name, const GameBoard& board) {
    if (name.empty()) {
        std::cerr << "Ошибка сохранения: Имя файла не может быть пустым." << std::endl;
        return false;
    }
    
    if (name.find('/') != std::string::npos || name.find('\\') != std::string::npos) {
        std::cerr << "Ошибка сохранения: Имя файла содержит недопустимые символы." << std::endl;
        return false;
    }

    if (!EnsureDir(dir)) {
        std::cerr << "Ошибка сохранения: Не удалось создать директорию '" << dir << "'." << std::endl;
        return false;
    }
    
    std::string path = dir + "/" + name + ".json";
    
    try {
        json j;
        
        j["rows"] = board.rows();
        j["cols"] = board.cols();
        j["level"] = board.current_level();

        j["cells"] = json::array();
        for (int r = 0; r < board.rows(); ++r) {
            json row = json::array();
            for (int c = 0; c < board.cols(); ++c) {
                row.push_back((int)board.cell_type(r, c));
            }
            j["cells"].push_back(row);
        }
        
        const Player& p = board.player();
        Position pp = board.PlayerPosition();
        j["player"] = {
            {"health", p.health()},
            {"max_health", p.max_health()},
            {"score", p.score()},
            {"weapon", p.weapon().type() == WeaponType::Sword ? "Sword" : "Bow"},
            {"has_trap", p.has_trap()},
            {"pos_r", pp.row()},
            {"pos_c", pp.col()}
        };
        
        if (p.hand().has_spell()) {
            j["hand"] = SpellTypeToStr(p.hand().active_spell()->type());
        } else {
            j["hand"] = nullptr;
        }
        
        j["inventory"] = json::array();
        for (int i = 0; i < p.inventory().item_count(); ++i) {
            const InventoryItem* it = p.inventory().get_item(i);
            if (it->type() == ItemType::Spell && it->spell()) {
                j["inventory"].push_back(SpellTypeToStr(it->spell()->type()));
            }
        }
        
        j["enemies"] = json::array();
        for (size_t i = 0; i < board.enemies().size(); ++i) {
            Position ep = board.EnemyPosition(i);
            j["enemies"].push_back({
                {"h", board.enemies()[i].health()},
                {"r", ep.row()},
                {"c", ep.col()}
            });
        }
        
        j["friends"] = json::array();
        for (size_t i = 0; i < board.friends().size(); ++i) {
            Position fp = board.FriendPosition(i);
            j["friends"].push_back({
                {"h", board.friends()[i].health()},
                {"r", fp.row()},
                {"c", fp.col()}
            });
        }
        
        j["towers"] = json::array();
        for (size_t i = 0; i < board.enemy_towers().size(); ++i) {
            Position tp = board.TowerPosition(i);
            j["towers"].push_back({
                {"h", board.enemy_towers()[i].health()},
                {"r", tp.row()},
                {"c", tp.col()},
                {"spawn_i", board.enemy_towers()[i].spawn_interval()}
            });
        }
        
        std::ofstream out(path);
        if (!out) {
            std::cerr << "Ошибка сохранения: Не удалось открыть файл '" << path << "' для записи." << std::endl;
            return false;
        }
        
        out << j.dump(2);
        
        if (!out.good()) {
            std::cerr << "Ошибка сохранения: Ошибка записи в файл '" << path << "'." << std::endl;
            return false;
        }
        
        std::cout << "Игра успешно сохранена в '" << path << "'." << std::endl;
        return true;
        
    } catch (const json::exception& e) {
        std::cerr << "Ошибка сохранения: JSON ошибка - " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка сохранения: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::Load(const std::string& dir, const std::string& name, GameBoard& board) {
    if (name.empty()) {
        std::cerr << "Ошибка загрузки: Имя файла не может быть пустым." << std::endl;
        return false;
    }
    
    std::string path = dir + "/" + name + ".json";

    std::ifstream in(path);
    if (!in) {
        std::cerr << "Ошибка загрузки: Файл '" << path << "' не существует или не может быть открыт." << std::endl;
        return false;
    }
    
    json j;
    try {
        in >> j;
    } catch (const json::parse_error& e) {
        std::cerr << "Ошибка загрузки: Некорректный JSON в файле '" << path << "'" << std::endl;
        std::cerr << "Детали: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки: " << e.what() << std::endl;
        return false;
    }

    try {
        if (!j.contains("rows") || !j.contains("cols") || !j.contains("player") || 
            !j.contains("cells") || !j.contains("enemies")) {
            std::cerr << "Ошибка загрузки: В файле отсутствуют обязательные поля." << std::endl;
            return false;
        }

        if (!j["rows"].is_number_integer() || !j["cols"].is_number_integer()) {
            std::cerr << "Ошибка загрузки: Некорректный формат размеров доски." << std::endl;
            return false;
        }

        int rows = j["rows"].get<int>();
        int cols = j["cols"].get<int>();

        if (rows != board.rows() || cols != board.cols()) {
            std::cerr << "Предупреждение: Размеры доски в сохранении (" << rows << "x" << cols 
                        << ") отличаются от текущих (" << board.rows() << "x" << board.cols() << ")." << std::endl;
        }

        if (j.contains("level") && j["level"].is_number_integer()) {
            board.set_level(j["level"].get<int>());
        }

        board.enemies().clear();
        board.friends().clear();
        board.enemy_towers().clear();
        board.player().hand().clear();
        
        for (int r = 0; r < board.rows(); ++r) {
            for (int c = 0; c < board.cols(); ++c) {
                board.SetCell(Position(r, c), CellType::Empty);
            }
        }
        
        if (!j["cells"].is_array()) {
            std::cerr << "Ошибка загрузки: Поле 'cells' должно быть массивом." << std::endl;
            return false;
        }

        for (int r = 0; r < rows && r < board.rows(); ++r) {
            if (r >= j["cells"].size() || !j["cells"][r].is_array()) {
                std::cerr << "Предупреждение: Некорректная строка " << r << " в клетках." << std::endl;
                continue;
            }
            
            for (int c = 0; c < cols && c < board.cols(); ++c) {
                if (c >= j["cells"][r].size() || !j["cells"][r][c].is_number_integer()) {
                    continue;
                }
                
                int cell_val = j["cells"][r][c].get<int>();
                if (cell_val < 0 || cell_val > 7) {
                    std::cerr << "Предупреждение: Некорректный тип клетки [" << r << "][" << c << "]: " << cell_val << std::endl;
                    continue;
                }
                
                CellType ct = (CellType)cell_val;
                if (ct == CellType::Wall || ct == CellType::Freeze || ct == CellType::Trap) {
                    board.SetCell(Position(r, c), ct);
                }
            }
        }

        if (!j["player"].is_object()) {
            std::cerr << "Ошибка загрузки: Поле 'player' должно быть объектом." << std::endl;
            return false;
        }

        if (!j["player"].contains("health") || !j["player"].contains("score") || 
            !j["player"].contains("pos_r") || !j["player"].contains("pos_c")) {
            std::cerr << "Ошибка загрузки: Отсутствуют обязательные поля игрока." << std::endl;
            return false;
        }

        board.player().set_health(j["player"]["health"].get<int>());
        int score = j["player"]["score"].get<int>();
        board.player().add_score(score - board.player().score());
        
        if (j["player"].contains("has_trap")) {
            board.player().set_trap(j["player"]["has_trap"].get<bool>());
        }

        board.player().set_trap(j["player"]["has_trap"].get<bool>());

        if (j["player"].contains("weapon")) {
            std::string weapon = j["player"]["weapon"].get<std::string>();
            if ((weapon == "Bow" && board.player().weapon().type() == WeaponType::Sword) ||
                (weapon == "Sword" && board.player().weapon().type() == WeaponType::Bow)) {
                board.player().change_weapon();
            }
        }
        
        int pr = j["player"]["pos_r"].get<int>();
        int pc = j["player"]["pos_c"].get<int>();

        if (pr < 0 || pr >= board.rows() || pc < 0 || pc >= board.cols()) {
            std::cerr << "Ошибка загрузки: Позиция игрока вне границ доски." << std::endl;
            return false;
        }
        board.AddPlayer(Position(pr, pc));
        
        if (j["enemies"].is_array()) {
            for (size_t i = 0; i < j["enemies"].size(); ++i) {
                if (!j["enemies"][i].is_object()) continue;

                int h = j["enemies"][i]["h"].get<int>();
                int r = j["enemies"][i]["r"].get<int>();
                int c = j["enemies"][i]["c"].get<int>();

                if (r < 0 || r >= board.rows() || c < 0 || c >= board.cols()) {
                    std::cerr << "Предупреждение: Враг #" << i << " имеет некорректную позицию." << std::endl;
                    continue;
                }

                board.AddEnemy(Position(r, c));
                board.DamageEnemy(i, 100 - h);
            }
        }
        if (j.contains("friends") && j["friends"].is_array()) {
            for (size_t i = 0; i < j["friends"].size(); ++i) {
                if (!j["friends"][i].is_object()) continue;

                int h = j["friends"][i]["h"].get<int>();
                int r = j["friends"][i]["r"].get<int>();
                int c = j["friends"][i]["c"].get<int>();

                if (r < 0 || r >= board.rows() || c < 0 || c >= board.cols()) {
                    std::cerr << "Предупреждение: Союзник #" << i << " имеет некорректную позицию." << std::endl;
                    continue;
                }

                board.AddFriend(Position(r, c));
                board.DamageFriend(i, 50 - h);
            }
        }
        if (j.contains("towers") && j["towers"].is_array()) {
            for (size_t i = 0; i < j["towers"].size(); ++i) {
                if (!j["towers"][i].is_object()) continue;
                
                int h = j["towers"][i]["h"].get<int>();
                int r = j["towers"][i]["r"].get<int>();
                int c = j["towers"][i]["c"].get<int>();
                int spawn_i = j["towers"][i]["spawn_i"].get<int>();

                if (r < 0 || r >= board.rows() || c < 0 || c >= board.cols()) {
                    std::cerr << "Предупреждение: Башня #" << i << " имеет некорректную позицию." << std::endl;
                    continue;
                }

                board.AddEnemyTower(Position(r, c), h, spawn_i);
            }
        }

        if (j.contains("hand") && !j["hand"].is_null() && j["hand"].is_string()) {
            std::string hand_type = j["hand"].get<std::string>();
            Spell* spell = CreateSpellByName(hand_type);
            if (spell) {
                board.player().hand().equip_spell(spell);
            }
        }
        
        if (j.contains("inventory") && j["inventory"].is_array()) {
            for (size_t i = 0; i < j["inventory"].size(); ++i) {
                if (!j["inventory"][i].is_string()) continue;
                
                std::string spell_name = j["inventory"][i].get<std::string>();
                Spell* spell = CreateSpellByName(spell_name);
                if (spell) {
                    board.player().inventory().add_spell(spell);
                }
            }
        }
        std::cout << "Игра успешно загружена из '" << path << "'." << std::endl;
        return true;
    } catch (const json::exception& e) {
        std::cerr << "Ошибка загрузки: JSON ошибка - " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки: " << e.what() << std::endl;
        return false;
    }
}