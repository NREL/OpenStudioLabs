#ifndef DLLDEMO_B_HPP
#define DLLDEMO_B_HPP

#include <b_export.h>
#include <iostream>
#include <memory>

namespace b {

class SingleB;

class B_EXPORT B {
  friend class SingleB;

private:
  B() = default;
};

class B_EXPORT SingleB {
public:
  static B &instance() {
    if (!b) {
      b = std::shared_ptr<B>(new B());
    }
    return *b;
  }

  static std::shared_ptr<B> b;
};

void PrintAddressB();

} // namespace b

#endif // DLLDEMO_B_HPP
