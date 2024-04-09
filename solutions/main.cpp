#include "world.hpp"

#include <chrono>
#include <iostream>
#include <thread>

int main() {
  int size{20};
  double start_black_percentage{0.2};
  double start_white_percentage{0.2};
  int max_age{25};
  World world(size, start_black_percentage, start_white_percentage, max_age);
  std::cout << "Initial global temperature: " << world.global_temperature()
            << '\n';
  int it{0};
  int num_iterations{100};
  while (it++ < num_iterations) {
    std::cout << "\033c";
    world.step(1.);
    world.print();
    std::cout << "Global temperature after " << it
              << " time step(s): " << world.global_temperature() << '\n';
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return 0;
}
