#ifndef ISPAWNER_H
#define ISPAWNER_H

class ISpawner {
public:
	virtual ~ISpawner() = default;
	
	virtual int timeToSpawn() const = 0;
};

#endif
