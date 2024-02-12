#include "a.hpp"
#include "b.hpp"

namespace a {

std::shared_ptr<A> SingleA::a;
int A::value = 0;

void PrintAddressA() {
  std::cout << "SingleA's instance is located at: " << &(a::SingleA::instance())
            << std::endl;
  std::cout << "SingleA's value is: " << a::SingleA::instance().value
            << std::endl;
}

void PrintAddressB() {
  std::cout << "SingleB's instance is located at: " << &(b::SingleB::instance())
            << std::endl;
  std::cout << "SingleB's value is: " << b::SingleB::instance().value
            << std::endl;
}

} // namespace a
