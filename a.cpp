#include "a.hpp"
#include "b.hpp"

namespace a {

std::shared_ptr<A> SingleA::a;

void PrintAddressA() {
  std::cout << "SingleA's instance is located at: " << &(a::SingleA::instance())
            << std::endl;
}

void PrintAddressB() {
  std::cout << "SingleB's instance is located at: " << &(b::SingleB::instance())
            << std::endl;
}

} // namespace a
