#include "World.h"

void World::Init(Field& field, std::shared_ptr<Player> player, int level, int count_enemies, int count_buildings, int count_towers, int count_allies) {
	level_ = level;

	playerManager_.SpawnPlayer(field, player);
	enemyManager_.SpawnEntities(field, count_enemies, level);
	buildingManager_.SpawnEnemiesBuildings(field, count_buildings, level);
	towerManager_.SpawnEnemiesTowers(field, count_towers, level);
	allyManager_.SpawnEntities(field, count_allies, level);
}

void World::Update(Field& field, std::shared_ptr<Player> player) {
	playerManager_.ManagePlayerTurn(player, field, *this);
	allyManager_.FindAllEntities(field);
	allyManager_.EntitiesTurn(field);
	enemyManager_.EntitiesTurn(field, player);
	buildingManager_.EnemiesBuildingTurn(field, *this);
	towerManager_.EnemiesTowersTurn(field);

}

void World::DeleteDeadEntites(Field& field) {
	enemyManager_.DeleteDeadEntities(field);
	buildingManager_.DeleteDeadEnemiesBuildings(field);
	towerManager_.DeleteDeadEnemiesTowers(field);
	allyManager_.DeleteDeadEntities(field);
}

PlayerManager& World::PlayerMan() { return playerManager_; }
EntityManager<Enemy>& World::Enemies() { return enemyManager_; }
EnemyBuildingManager& World::EnemiesBuildings() { return buildingManager_; }
EnemyTowerManager& World::EnemiesTowers() { return towerManager_; }
EntityManager<Ally>& World::Allyes() { return allyManager_; }

TokenGameState World::SerializeState(Field& field, std::shared_ptr<Player> player) {
	TokenGameState state;

	state.level = level_;

	// === PLAYER ===
	state.player = player->serialis();
	field.GetPosEntity(player, state.player_x, state.player_y);



	// === FIELD ===
	int width, height;
	field.GetSize(width, height);
	state.field.width = width;
	state.field.height = height;
	state.field.cells.resize(height);

	for (int i = 0; i < height; ++i) {
		state.field.cells[i].resize(width);
		for (int j = 0; j < width; ++j) {
			auto cell = field.GetCell(j, i);
			TokenCell& token_cell = state.field.cells[i][j];

			// Cell type
			token_cell.type = cell->GetName();

			// Event
			token_cell.has_event = (cell->GetEvent() != nullptr);
			if (token_cell.has_event) {
				auto event = cell->GetEvent();
				if (auto trap = dynamic_cast<TrapEvent*>(event.get())) {
					token_cell.event.type = "Trap";
					token_cell.event.damage = 10; // можно добавить getTrapDamage()
				}
			}

			// Entity
			auto entity = cell->GetEntity();
			token_cell.has_entity = (entity != nullptr && entity != player);

			if (token_cell.has_entity) {
				token_cell.entity = entity->serialise();
			}
		}
	}

	return state;
}

void World::LoadState(Field& field, std::shared_ptr<Player>& player, const TokenGameState& state) {

    int W = state.field.width;
    int H = state.field.height;

    field = Field(W, H, 0, 0);

	enemyManager_ = EntityManager<Enemy>();
    allyManager_ = EntityManager<Ally>();
    buildingManager_ = EnemyBuildingManager();
    towerManager_ = EnemyTowerManager();

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            const TokenCell& token_cell = state.field.cells[y][x];

            field.ReplaceCellWithType(x, y, token_cell.type);

            auto cell = field.GetCell(x, y);

            if (token_cell.has_event && token_cell.event.type == "Trap") {
                auto trap = std::make_shared<TrapEvent>(token_cell.event.damage);
                cell->SetEvent(trap);
            }
        	if (token_cell.has_entity) {
                std::shared_ptr<Entity> entity;

                if (token_cell.entity.type == "Enemy") {
                    entity = std::make_shared<Enemy>();
                    entity->CauseDamage(100 - token_cell.entity.hp);
                    enemyManager_.AddEntity(std::dynamic_pointer_cast<Enemy>(entity));

                } else if (token_cell.entity.type == "Ally") {
                    entity = std::make_shared<Ally>();
                    entity->CauseDamage(100 - token_cell.entity.hp);
                    allyManager_.AddEntity(std::dynamic_pointer_cast<Ally>(entity));

                } else if (token_cell.entity.type == "EnemyBuilding") {
                    entity = std::make_shared<EnemyBuilding>(token_cell.entity.steps_to_spawn);
                    entity->CauseDamage(100 - token_cell.entity.hp);
                    buildingManager_.AddEntity(std::dynamic_pointer_cast<EnemyBuilding>(entity));

                } else if (token_cell.entity.type == "EnemyTower") {
                    int spell_damage = 0;
                    int spell_range = 0;
                    try {
                        if (!token_cell.entity.spell.damage.empty())
                            spell_damage = std::stoi(token_cell.entity.spell.damage);
                        if (!token_cell.entity.spell.range.empty())
                            spell_range = std::stoi(token_cell.entity.spell.range);
                    } catch (...) {
                        spell_damage = 5;
                        spell_range = 2;
                    }
                    entity = std::make_shared<EnemyTower>(spell_range, spell_damage);
                    entity->CauseDamage(100 - token_cell.entity.hp);
                    towerManager_.AddEntity(std::dynamic_pointer_cast<EnemyTower>(entity));
                }
            	if (entity) {
                    bool placed = field.SetEntity(entity, x, y);
                    if (!placed) {
                        if (field.GetCell(x, y)->GetName() == "Impassable") {
                            field.ReplaceCellWithType(x, y, "Cell");
                            field.SetEntity(entity, x, y);
                        }
                    }
                }
            }
        }
    }

	level_ = state.level;
	player = Player::deserialise(state.player);

	bool spawned = field.SetEntity(player, state.player_x, state.player_y);
	if (!spawned) {
		std::cerr << "Warning: Player spawn failed at (" << state.player_x << "," << state.player_y
				  << "). Forcing spawn at (0,0)\n";
		field.SetEntity(player, 0, 0);
	}
}


