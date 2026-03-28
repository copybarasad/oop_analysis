#ifndef LEVEL_TEMPLATE
#define LEVEL_TEMPLATE

struct LevelTemplate
{
	int widthField, heightField, levelCompletePoints;
	std::pair<int, int> playerPos;
	std::pair<int, int> towerPos;
	std::pair<int, int> spawnerPos;
	int towerDebuffToDamage, towerHealth, towerRadius;
	int spawnerRadius, spawnerHealth, spawnerCoolDown;
	int enemiesBuffDamage;

	std::map<TypeOfCell, std::vector<std::pair<int, int>>> unusualCellPositions;

	LevelTemplate(int w, int h, int lvlPoints, std::pair<int, int> pPos, std::pair<int, int> tPos, std::pair<int, int> sPos, \
		int tDebuff, int tHealth, int tRad, int sRad, int sHealth, int sCoolDown, int eBuff, std::map<TypeOfCell, std::vector<std::pair<int, int>>> unusualCellPos)
		: widthField(w),
		heightField(h),
		levelCompletePoints(lvlPoints),
		playerPos(pPos),
		towerPos(tPos),
		spawnerPos(sPos),
		towerDebuffToDamage(tDebuff),
		towerHealth(tHealth),
		towerRadius(tRad),
		spawnerRadius(sRad),
		spawnerHealth(sHealth),
		spawnerCoolDown(sCoolDown),
		enemiesBuffDamage(eBuff),
		unusualCellPositions(unusualCellPos.begin(), unusualCellPos.end())
	{
	}
};

#endif