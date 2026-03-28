#ifndef ISLOWABLE_H
#define ISLOWABLE_H

class ISlowable {
public:
	virtual ~ISlowable() = default;
	
	virtual bool isSlowed() const = 0;
	virtual void setSlow(bool slowed) = 0;
};

#endif
