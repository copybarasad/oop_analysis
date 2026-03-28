#ifndef PLAYER_RESOURCES_H
#define PLAYER_RESOURCES_H

class PlayerResources {
private:
	int points;
	int kills;

public:
	PlayerResources();
	
	void addPoints(int amount);
	bool spendPoints(int amount);
	int getPoints() const;
	void setPoints(int amount);
	
	void incrementKills();
	int getKills() const;
	void setKills(int amount);
};

#endif
