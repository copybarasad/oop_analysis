#include "SaveManager.h"

bool SaveManager::SaveGame(const std::string& filename, GameWorld& world) {
  const std::string fullpath = "saves/" + EnsureJsonExtension(filename);

  if (!std::filesystem::exists("saves"))
    std::filesystem::create_directories("saves");

  std::ofstream file(fullpath);
  if (!file.is_open()) throw FileWriteException(fullpath);

  nlohmann::json j;

  {
    PlayerSaveData data = world.GetPlayer().Save();

    j["player"]["hp"] = data.hp;
    j["player"]["max_hp"] = data.hp;
    j["player"]["damage"] = data.damage;
    j["player"]["score"] = data.score;
    j["player"]["mode"] = static_cast<int>(data.mode);
    j["player"]["pendingBuffs"] = data.pendingBuffs;

    auto pos = world.GetPlayer().GetPosition();
    j["player"]["pos"] = {{"x", pos.x}, {"y", pos.y}};

    auto& hand = world.GetPlayer().GetHand();
    j["player"]["hand_size"] = hand.MaxSize();
    j["player"]["hand"] = nlohmann::json::array();
    for (const auto& spell : hand.GetSpells())
      j["player"]["hand"].push_back(spell->GetName());
  }

  const auto& map = world.GetMap();
  j["map"]["width"] = map.GetWidth();
  j["map"]["height"] = map.GetHeight();

  j["map"]["cells"] = nlohmann::json::array();
  for (int y = 0; y < map.GetHeight(); ++y) {
    nlohmann::json row = nlohmann::json::array();
    for (int x = 0; x < map.GetWidth(); ++x)
      row.push_back(static_cast<int>(map.GetCellType(x, y)));
    j["map"]["cells"].push_back(row);
  }

  j["map"]["level"] = world.GetCurrentLevel();

  j["enemies"] = nlohmann::json::array();
  for (const auto& e : world.GetEnemies()) {
    j["enemies"].push_back(
        {{"hp", e->GetHealth()},
         {"damage", e->GetDamage()},
         {"pos", {{"x", e->GetPosition().x}, {"y", e->GetPosition().y}}}});
  }

  j["spawners"] = nlohmann::json::array();
  for (const auto& s : world.GetSpawners()) {
    j["spawners"].push_back(
        {{"pos", {{"x", s->GetPosition().x}, {"y", s->GetPosition().y}}}});
  }

  j["towers"] = nlohmann::json::array();
  for (const auto& t : world.GetTowers()) {
    j["towers"].push_back(
        {{"pos", {{"x", t->GetPosition().x}, {"y", t->GetPosition().y}}},
         {"hp", t->GetHealth()}});
  }

  j["traps"] = nlohmann::json::array();
  for (const auto& tr : world.GetTraps()) {
    j["traps"].push_back(
        {{"pos", {{"x", tr->GetPosition().x}, {"y", tr->GetPosition().y}}},
         {"damage", tr->GetDamage()}});
  }

  j["allies"] = nlohmann::json::array();
  for (const auto& a : world.GetAllies()) {
    j["allies"].push_back(
        {{"pos", {{"x", a->GetPosition().x}, {"y", a->GetPosition().y}}},
         {"hp", a->GetHealth()}});
  }

  std::string _t = j.dump(2);     
  for (size_t i = 0; i < _t.size(); ++i)
      _t[i] ^= 0x5A;     
  file.write(_t.data(), _t.size());


  return true;
}
