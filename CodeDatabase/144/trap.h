#ifndef TRAP_H
#define TRAP_H

class TrapActionContext;

class Trap {
private:
    int positionX;
    int positionY;
    int damage;
    bool isActive;

public:
	struct Snapshot {
		int positionX = -1;
		int positionY = -1;
		int damage = 0;
		bool active = false;
	};

    Trap(int x, int y, int trapDamage);
    
    int getPositionX() const;
    int getPositionY() const;
    int getDamage() const;
    bool getIsActive() const;
    void checkAndTrigger(TrapActionContext& context);
    void setPosition(int x, int y);
    void setDamage(int value);
    void setActive(bool activeState);
    void deactivate();
	Snapshot createSnapshot() const;
	static Trap fromSnapshot(const Snapshot& snapshot);
};

#endif