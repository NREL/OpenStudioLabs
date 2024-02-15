#include "DynamicLibrary.hpp"
#include "a.hpp"
#include "b.hpp"
#include "path.hpp"

#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "SingleA's instance is located at: " << &(a::SingleA::instance())
            << std::endl;

  a::PrintAddressA();

  a::SingleA::instance().value = 1;

  a::PrintAddressA();

  std::cout << "SingleB's instance is located at: " << &(b::SingleB::instance())
            << std::endl;

  a::PrintAddressB();

  b::PrintAddressB();

  b::SingleB::instance().value = 1;

  DynamicLibrary lib_a(EXEPath().parent_path() / DLLFilename("a"));

  const auto print_address_a = lib_a.load_symbol<void()>("PrintAddressA");
  print_address_a();

  const auto print_address_b = lib_a.load_symbol<void()>("PrintAddressB");
  print_address_b();

  return 0;
}
