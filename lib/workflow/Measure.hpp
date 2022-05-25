#ifndef MEASURE_HPP
#define MEASURE_HPP

#include <string>
#include <string_view>
#include <iostream>
#include <vector>

namespace openstudio {

class Runner;
class Model;

class Measure
{
 public:
  Measure() = default;
  Measure(const Measure&) = delete;
  Measure(Measure&&) = delete;
  Measure& operator=(const Measure&) = delete;
  Measure& operator=(Measure&&) = delete;
  virtual ~Measure() = default;

  virtual std::string name() = 0;
  virtual bool run(Runner&) = 0;
};

}  // Namespace openstudio

#endif  // ifndef MEASURE_HPP
