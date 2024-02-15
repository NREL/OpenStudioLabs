#include "b.hpp"

namespace b {

// std::shared_ptr<B> SingleB::b;
int B::value = 0;

void PrintAddressB() {
  std::cout << "SingleB's instance is located at: " << &(b::SingleB::instance())
            << std::endl;
  std::cout << "SingleB's value is: " << b::SingleB::instance().value
            << std::endl;
}

} // namespace b
