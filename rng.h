#pragma once
#include <random>
#include <chrono>

using namespace std;

class Rng {
  public:
  Rng() {
    rng=mt19937(chrono::system_clock::now().time_since_epoch().count());
  }

  generate(int l,int h) {
    uniform_int_distribution<mt19937::result_type> dist(l,h);
    return dist(rng);
  }

  private:
  mt19937 rng;
};
