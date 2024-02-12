#ifndef DLLDEMO_B_HPP
#define DLLDEMO_B_HPP

#include <iostream>
#include <memory>

namespace b {

class SingleB {
public:
  static SingleB &instance() {
    if (!single_b) {
      single_b = std::shared_ptr<SingleB>(new SingleB());
    }
    return *single_b;
  }

  static std::shared_ptr<SingleB> single_b;

private:
  SingleB() = default;
};

void PrintAddressB();

} // namespace b

#endif // DLLDEMO_B_HPP
