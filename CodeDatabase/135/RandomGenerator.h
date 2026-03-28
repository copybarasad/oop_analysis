#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <random>

class RandomGenerator {
private:
	mutable std::mt19937 rng;
	
public:
	RandomGenerator();
	
	explicit RandomGenerator(unsigned int seed);
	
	int getInt(int min, int max) const;
	
	double getDouble(double min, double max) const;
	
	bool getBool(double probability = 0.5) const;
};

#endif
