#ifndef DLLDEMO_A_HPP
#define DLLDEMO_A_HPP

#include <a_export.h>
#include <iostream>
#include <memory>

namespace a {

class SingleA;

class A_EXPORT A {
  friend class SingleA;

public:
  static int value;

private:
  A() = default;
};

class SingleA {
public:
  A_EXPORT static A &instance() {
    if (!a) {
      a = std::shared_ptr<A>(new A());
    }
    return *a;
  }

  static std::shared_ptr<A> a;
};

A_EXPORT void PrintAddressA();

A_EXPORT void PrintAddressB();

} // namespace a

#endif // DLLDEMO_A_HPP
