#ifndef DLLDEMO_A_HPP
#define DLLDEMO_A_HPP

#include "api.hpp"
#include "singleton.hpp"
#include <a_export.h>
#include <iostream>
#include <memory>

namespace a {

// class SingleA;

class A_EXPORT A {
  friend class Singleton<A>;

public:
  int value;

protected:
  A() = default;
};

// class SingleA {
// public:
//   A_EXPORT static A &instance() {
//     static A a;
//     return a;
//     // if (!a) {
//     //   a = std::shared_ptr<A>(new A());
//     // }
//     // return *a;
//   }
//
//   // static std::shared_ptr<A> a;
// };

extern "C" {
A_EXPORT void PrintAddressA();

A_EXPORT void PrintAddressB();
}

} // namespace a

DLLDEMO_TEMPLATE_EXT template class DLLDEMO_API Singleton<a::A>;

namespace a {

using SingleA = Singleton<A>;

}

#endif // DLLDEMO_A_HPP
