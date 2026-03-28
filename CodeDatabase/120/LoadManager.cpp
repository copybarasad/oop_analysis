#include "LoadManager.h"

bool LoadManager::LoadGame(const std::string& filename, GameWorld& world) {
  const std::string fullpath = "saves/" + EnsureJsonExtension(filename);

  if (!std::filesystem::exists(fullpath)) throw FileNotFoundException(fullpath);

  std::ifstream file(fullpath);
  if (!file.is_open()) throw FileReadException(fullpath);

  std::string _t((std::istreambuf_iterator<char>(file)),
                  std::istreambuf_iterator<char>());

  for (size_t i = 0; i < _t.size(); ++i)
      _t[i] ^= 0x5A;                

  nlohmann::json j = nlohmann::json::parse(_t);

  if (!j.contains("player") || !j.contains("map"))
    throw InvalidJsonException("Missing player or map section");

  {
    PlayerSaveData data;
    data.hp = j["player"].value("hp", 100);
    data.max_hp = j["player"].value("max_hp", 100);
    data.damage = j["player"].value("damage", 10);
    data.score = j["player"].value("score", 0);
    data.mode = static_cast<SavedCombatMode>(j["player"].value("mode", 0));
    data.pendingBuffs = j["player"].value("pendingBuffs", 0);

    world.GetPlayer().LoadFromSave(data);

    int px = j["player"]["pos"].value("x", 0);
    int py = j["player"]["pos"].value("y", 0);
    world.GetPlayer().SetPosition({px, py});

    int hs = j["player"].value("hand_size", 3);
    Hand hand(hs);
    hand.SetMaxSize(hs);

    if (j["player"].contains("hand")) {
      SpellFactory factory;
      for (auto& name : j["player"]["hand"]) {
        try {
          auto spell = factory.CreateSpell(name.get<std::string>());
          if (spell) hand.AddSpell(std::move(spell));
        } catch (...) {
          std::cerr << "Warning: Failed to load spell: "
                    << name.get<std::string>() << '\n';
        }
      }
    }
    world.GetPlayer().SetHand(std::move(hand));
  }

  auto& map = world.GetMap();
  size_t width = j["map"].value("width", 0);
  size_t height = j["map"].value("height", 0);

  if (j["map"]["cells"].size() != height)
    throw InvalidJsonException("Map row mismatch");

  std::vector<std::vector<Cell>> cells(height, std::vector<Cell>(width));
  for (size_t y = 0; y < height; ++y) {
    if (j["map"]["cells"][y].size() != width)
      throw InvalidJsonException("Map column mismatch");
    for (size_t x = 0; x < width; ++x)
      cells[y][x].SetType(
          static_cast<Cell::CellType>(j["map"]["cells"][y][x].get<int>()));
  }
  map.SetCells(std::move(cells));
  world.SetCurrentLevel(j["map"].value("level", 1));

  world.GetEnemies().clear();
  for (auto& je : j["enemies"]) {
    Position p{je["pos"].value("x", 0), je["pos"].value("y", 0)};
    world.GetEnemies().push_back(
        std::make_unique<Enemy>(je.value("hp", 10), je.value("damage", 1), p));
  }

  world.GetSpawners().clear();
  for (auto& js : j["spawners"]) {
    Position p{js["pos"].value("x", 0), js["pos"].value("y", 0)};
    world.GetSpawners().push_back(std::make_unique<EnemySpawner>(p));
  }

  world.GetTowers().clear();
  for (auto& jt : j["towers"]) {
    Position p{jt["pos"].value("x", 0), jt["pos"].value("y", 0)};
    int hp = jt.value("hp", 10);
    world.GetTowers().push_back(std::make_unique<Tower>(p, hp));
  }

  world.GetTraps().clear();
  for (auto& jt : j["traps"]) {
    Position p{jt["pos"].value("x", 0), jt["pos"].value("y", 0)};
    int dmg = jt.value("damage", 1);
    world.AddTrap(std::make_unique<Trap>(p, dmg));
  }

  world.GetAllies().clear();
  for (auto& ja : j["allies"]) {
    Position p{ja["pos"].value("x", 0), ja["pos"].value("y", 0)};
    int hp = ja.value("hp", 5);
    world.AddSummonedAlly(std::make_unique<SummonedAlly>(p));
    world.GetAllies().back()->SetHealth(hp);
  }

  return true;
}
