#include "b.hpp"

namespace b {

std::shared_ptr<B> SingleB::b;

void PrintAddressB() {
  std::cout << "SingleB's instance is located at: " << &(b::SingleB::instance())
            << std::endl;
}

} // namespace b
