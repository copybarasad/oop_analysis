#include "extras.h"
#include <random>
unsigned int rng(int a, int b) {
  int less = a > b ? b : a;
  int more = a > b ? a : b;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<std::mt19937::result_type> res(less, more);
  return res(mt);
}
