#include "b.hpp"

namespace b {

std::shared_ptr<SingleB> SingleB::single_b;

void PrintAddressB() {
  std::cout << "SingleB's instance is located at: " << &(b::SingleB::instance())
            << std::endl;
}

} // namespace b
