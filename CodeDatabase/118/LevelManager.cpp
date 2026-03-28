#include "LevelManager.h"

LevelManager::LevelManager(const std::vector<LevelTemplate>& temps)
	: templates(temps) {}

void LevelManager::RemoveRandomHalfSpells(std::vector<std::shared_ptr<ISpell>>& spells, std::vector<int>& indexes)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(indexes.begin(), indexes.end(), gen);

	for (auto i = 0; i < indexes.size() / 2; ++i)
		spells.pop_back();
}

void LevelManager::LoadNextLevel(const PlayerConfig& config, GameLogic& logic)
{
	LevelTemplate temp = templates[config.currLvl];
	
	logic.SetGameField(temp.widthField, temp.heightField, temp.unusualCellPositions);
	logic.SetEnemySpawner(temp.spawnerPos, temp.spawnerHealth, temp.spawnerCoolDown, temp.spawnerRadius);
	logic.SetTower(temp.towerPos, temp.towerHealth, temp.towerRadius, temp.towerDebuffToDamage, true);
	logic.SetTableOfSpells(config.spellRadius, config.spellBuff);

	Hand hand = config.playerHand;

	if (config.currLvl == 0)
		hand.AddSpell(std::make_shared<SpellOfDirectDamage>(logic.GetGameField()));

	logic.SetPlayer(config.playerHealth, config.playerPos, config.playerDamage, config.playerScore, config.attackType, hand);

}

void LevelManager::PrepareStateForNextLevel(PlayerConfig& config, GameLogic& logic)
{
	if (config.currLvl == templates.size() - 1)
		throw InputException("No next level");

	config.currLvl += 1;

	auto player = logic.GetPlayer();

	config.playerHealth = PLAYER_HEALTH;
	config.playerPos = templates[config.currLvl].playerPos;
	config.playerDamage = player->GetDamage();
	config.playerScore += player->GetScore();
	config.attackType = player->GetTypeOfAttack();

	Hand hand = player->GetHand();
	int handSize = hand.GetCurrSize();

	std::vector<std::shared_ptr<ISpell>> spells(handSize);
	std::vector<int> indexes(handSize);

	for (int i = 0; i < handSize; ++i)
	{
		spells[i] = hand.GetSpell(i);
		indexes[i] = i;
	}

	RemoveRandomHalfSpells(spells, indexes);

	Hand newHand = Hand();

	for (int i = 0; i < handSize / 2; ++i)
		newHand.AddSpell(spells[i]);

	config.playerHand = newHand;
}