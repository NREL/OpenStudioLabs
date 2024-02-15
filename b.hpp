#ifndef DLLDEMO_B_HPP
#define DLLDEMO_B_HPP

#include <b_export.h>
#include <iostream>
#include <memory>

namespace b {

class SingleB;

class B_EXPORT B {
  friend class SingleB;

public:
  static int value;

private:
  B() = default;
};

class SingleB {
public:
  B_EXPORT static B &instance() {
    static B b;
    return b;

    // if (!b) {
    //   b = std::shared_ptr<B>(new B());
    // }
    // return *b;
  }

  // static std::shared_ptr<B> b;
};

B_EXPORT void PrintAddressB();

} // namespace b

#endif // DLLDEMO_B_HPP
