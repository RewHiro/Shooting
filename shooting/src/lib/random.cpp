
//
// 乱数
//

#include "random.hpp"


Random::Random() :
  engine_(std::mt19937::default_seed),
  dist_zero_to_one_(0.0f, 1.0f)
{}


void Random::setSeed(const u_int new_seed) {
  engine_.seed(new_seed);
}
  
// [0, last) を返す
int Random::fromZeroToLast(const int last) {
  return engine_() % last;
}

// [first, last] を返す
int Random::fromFirstToLast(const int first, const int last) {
  return first + fromZeroToLast(last - first + 1);
}

  
// [0.0f, 1.0f] を返す
float Random::fromZeroToOne() {
  return dist_zero_to_one_(engine_);
}

// [first, last] を返す
float Random::fromFirstToLast(const float first, const float last) {
  return first + (last - first) * fromZeroToOne();
}
