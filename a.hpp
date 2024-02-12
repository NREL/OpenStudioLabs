#ifndef DLLDEMO_A_HPP
#define DLLDEMO_A_HPP

#include <a_export.h>
#include <iostream>
#include <memory>

namespace a {

class A_EXPORT SingleA {
public:
  static SingleA &instance() {
    if (!single_a) {
      single_a = std::shared_ptr<SingleA>(new SingleA());
    }
    return *single_a;
  }

  static std::shared_ptr<SingleA> single_a;

private:
  SingleA() = default;
};

A_EXPORT void PrintAddressA();

A_EXPORT void PrintAddressB();

} // namespace a

#endif // DLLDEMO_A_HPP
