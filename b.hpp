#ifndef DLLDEMO_B_HPP
#define DLLDEMO_B_HPP

#include "api.hpp"
#include "singleton.hpp"
#include <b_export.h>
#include <iostream>
#include <memory>

namespace b {

class B_EXPORT B {
  friend class Singleton<B>;

public:
  int value;

private:
  B() = default;
};

// class SingleB {
// public:
//   B_EXPORT static B &instance() {
//     static B b;
//     return b;
//
//     // if (!b) {
//     //   b = std::shared_ptr<B>(new B());
//     // }
//     // return *b;
//   }
//
//   // static std::shared_ptr<B> b;
// };

B_EXPORT void PrintAddressB();

} // namespace b

DLLDEMO_TEMPLATE_EXT template class DLLDEMO_API Singleton<b::B>;

namespace b {

using SingleB = Singleton<B>;

}

#endif // DLLDEMO_B_HPP
