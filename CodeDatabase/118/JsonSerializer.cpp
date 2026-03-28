#include "JsonSerializer.h"

json JsonSerializer::Serialize(PlayerConfig& config, GameLogic& logic)
{
	json j;
    auto player = logic.GetPlayer();

	j["current level"] = config.currLvl;
	j["player"]["health"] = player->GetHealth();
	j["player"]["position"] = player->GetPosition();
	j["player"]["damage"] = player->GetDamage();
    j["player"]["score"] = player->GetScore();
    j["player"]["type of attack"] = player->GetTypeOfAttack() == MELEE ? "melee" : "ranged";
	
	auto& hand = player->GetHand();

	j["player"]["hand"] = json::array();
	j["player"]["hand"].push_back({ "hand capacity", hand.GetCapacity() });
	
	for (int i = 0; i < hand.GetCurrSize(); ++i)
		j["player"]["hand"].push_back(hand.GetSpell(i)->GetTitle());

    auto spawner = logic.GetEnemySpawner();

    j["spawner"]["health"] = spawner->GetHealth();
    j["spawner"]["position"] = spawner->GetPosition();
    j["spawner"]["coolDown"] = spawner->GetCoolDown();
    j["spawner"]["radius of spawning"] = spawner->GetSpawnRadius();

    auto tower = logic.GetTower();

    j["tower"]["health"] = tower->GetHealth();
    j["tower"]["position"] = tower->GetPosition();
    j["tower"]["radius of attack"] = tower->GetAttackRadius();
    j["tower"]["dubuff to attack"] = tower->GetDebuff();
    j["tower"]["can attack"] = tower->CanAttack() ? std::string("Can") : std::string("Cannot");

    j["spell params"] = { { "spell radius", config.spellRadius }, { "spell buff", config.spellBuff } };

	auto field = logic.GetGameField();

	j["game field"]["width"] = field->GetWidth();
	j["game field"]["height"] = field->GetHeight();
	j["game field"]["impassable cells"] = json::array();
	j["game field"]["slowing cells"] = json::array();
	j["game field"]["trap cells"] = json::array();
    j["game field"]["enemies"] = json::array();

	for (int y = 0; y < field->GetHeight(); ++y)
	{
		for (int x = 0; x < field->GetWidth(); ++x)
		{
			const Cell currCell = field->GetCellByPosForReading({ x, y });

            std::shared_ptr<Entity> entity = currCell.GetCellContentForReading();

            if (entity != nullptr && entity->GetPosition() != player->GetPosition() && entity->GetPosition() != spawner->GetPosition() && entity->GetPosition() != tower->GetPosition())
                j["game field"]["enemies"].push_back({ { "position", {x, y} }, { "health", entity->GetHealth() }, { "damage", entity->GetDamage() } });

			if (currCell.GetTypeOfCell() == IMPASSABLE)
				j["game field"]["impassable cells"].push_back({ x, y });

			else if (currCell.GetTypeOfCell() == SLOWING)
				j["game field"]["slowing cells"].push_back({ x, y });

			else if (currCell.GetTypeOfCell() == TRAP)
				j["game field"]["trap cells"].push_back({ x, y });
		}
	}

    return j;
}

void JsonSerializer::Deserialize(PlayerConfig& config, GameLogic& logic, json j)
{
    if (j.contains("current level"))
        config.currLvl = j["current level"].get<int>();

    if (j.contains("spell params"))
    {
        const json& sParamsJObj = j["spell params"];

        if (sParamsJObj.contains("spell radius"))
            config.spellRadius = sParamsJObj["spell radius"].get<int>();

        if (sParamsJObj.contains("spell buff"))
            config.spellBuff = sParamsJObj["spell buff"].get<int>();

        logic.SetTableOfSpells(config.spellRadius, config.spellBuff);
    }

    if (j.contains("game field"))
    {
        int w = 10, h = 10;
        const json& fieldJObj = j["game field"];

        if (fieldJObj.contains("width"))
            w = fieldJObj["width"].get<int>();

        if (fieldJObj.contains("height"))
            h = fieldJObj["height"].get<int>();

        std::map<TypeOfCell, std::vector<std::pair<int, int>>> unusualCells;

        if (fieldJObj.contains("impassable cells"))
        {
            for (const auto& cellPos : fieldJObj["impassable cells"])
                unusualCells[IMPASSABLE].push_back(cellPos);
        }

        if (fieldJObj.contains("slowing cells"))
        {
            for (auto& cellPos : fieldJObj["slowing cells"])
                unusualCells[SLOWING].push_back(cellPos);
        }

        if (fieldJObj.contains("trap cells"))
        {
            for (auto& cellPos : fieldJObj["trap cells"])
                unusualCells[TRAP].push_back(cellPos);
        }

        std::vector<std::shared_ptr<Entity>> enemies;

        if (fieldJObj.contains("enemies"))
        {
            for (const auto& enemy : fieldJObj["enemies"])
                enemies.push_back(std::make_shared<Enemy>(enemy["position"].get<std::pair<int, int>>(), enemy["health"].get<int>(), enemy["damage"].get<int>()));
        }

        logic.SetGameField(w, h, unusualCells, enemies);
    }

    if (j.contains("player"))
    {
        const json& playerJObj = j["player"];

        if (playerJObj.contains("health"))
            config.playerHealth = playerJObj["health"].get<int>();
        
        if (playerJObj.contains("position"))
            config.playerPos = playerJObj["position"].get<std::pair<int, int>>();
        
        if (playerJObj.contains("damage"))
            config.playerDamage = playerJObj["damage"].get<int>();
        
        if (playerJObj.contains("score"))
            config.playerScore = playerJObj["score"].get<int>();

        if (playerJObj.contains("type of attack"))
            config.attackType = playerJObj["type of attack"].get<std::string>() == "melee" ? MELEE : RANGED;

        if (playerJObj.contains("hand"))
        {
            config.playerHand = Hand(playerJObj["hand"][0].get<std::pair<std::string, int>>().second);

            for (size_t i = 1; i < playerJObj["hand"].size(); ++i)
            {
                std::string spellTitle = playerJObj["hand"][i].get<std::string>();
                std::shared_ptr<ISpell> currSpell = nullptr;

                if (spellTitle == "Direct damage\n")
                    currSpell = std::make_shared<SpellOfDirectDamage>(logic.GetGameField(), config.spellRadius, config.spellBuff);

                else if (spellTitle == "Area damage\n")
                    currSpell = std::make_shared<SpellOfAreaDamage>(logic.GetGameField(), config.spellRadius, config.spellBuff);

                if (spellTitle == "Spawn trap\n")
                    currSpell = std::make_shared<SpellOfSpawningTrap>(logic.GetGameField(), config.spellRadius, config.spellBuff);

                config.playerHand.AddSpell(currSpell);
            }
        }
        logic.SetPlayer(config.playerHealth, config.playerPos, config.playerDamage, config.playerScore, config.attackType, config.playerHand);
    }

    if (j.contains("spawner"))
    {
        const json& spawnerJObj = j["spawner"];
        int health = SPAWNER_HEALTH, coolDown = SPAWNER_COOLDOWN, radius = SPAWNER_RADIUS;
        std::pair<int, int> pos(5, 5);
        
        if (spawnerJObj.contains("health"))
            health = spawnerJObj["health"].get<int>();

        if (spawnerJObj.contains("position"))
             pos = spawnerJObj["position"].get<std::pair<int, int>>();

        if (spawnerJObj.contains("coolDown"))
            coolDown = spawnerJObj["coolDown"].get<int>();
        
        if (spawnerJObj.contains("radius of spawning"))
            radius = spawnerJObj["radius of spawning"].get<int>();

        logic.SetEnemySpawner(pos, health, coolDown, radius);
    }

    if (j.contains("tower"))
    {
        const json& towerJObj = j["tower"];
        int health = TOWER_HEALTH, radius = TOWER_RADIUS, debuff = TOWER_DEBUFF;
        std::pair<int, int> pos(7, 7);
        bool canAttack = true;

        if (towerJObj.contains("health"))
            health = towerJObj["health"].get<int>();

        if (towerJObj.contains("position"))
            pos = towerJObj["position"].get<std::pair<int, int>>();

        if (towerJObj.contains("radius of attack"))
            radius = towerJObj["radius of attack"].get<int>();

        if (towerJObj.contains("debuff to attack"))
            debuff = towerJObj["debuff to attack"].get<int>();

        if (towerJObj.contains("can attack"))
            canAttack = towerJObj["can attack"].get<std::string>() == std::string("Can") ? true : false; 
        
        logic.SetTower(pos, health, radius, debuff, canAttack);
    }
}
