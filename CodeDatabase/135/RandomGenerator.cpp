#include "RandomGenerator.h"

RandomGenerator::RandomGenerator() : rng(std::random_device{}()) {}

RandomGenerator::RandomGenerator(unsigned int seed) : rng(seed) {}

int RandomGenerator::getInt(int min, int max) const {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(rng);
}

double RandomGenerator::getDouble(double min, double max) const {
	std::uniform_real_distribution<double> dist(min, max);
	return dist(rng);
}

bool RandomGenerator::getBool(double probability) const {
	std::bernoulli_distribution dist(probability);
	return dist(rng);
}
