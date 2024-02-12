#include "a.hpp"
#include "b.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

  std::cout << "SingleA's instance is located at: " << &(a::SingleA::instance())
            << std::endl;

  a::PrintAddressA();

  std::cout << "SingleB's instance is located at: " << &(b::SingleB::instance())
            << std::endl;

  a::PrintAddressB();

  b::PrintAddressB();

  return 0;
}
