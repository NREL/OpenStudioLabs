#ifndef MEASURE_HPP
#define MEASURE_HPP

#include "MeasureAPI.hpp"

#include <map>
#include <string>
#include <vector>

namespace openstudio {

class Runner;
class Model;
class OSArgument;
using OSArgumentMap = std::map<std::string, OSArgument>;

class MEASURE_API Measure
{
 public:
  Measure() = default;
  Measure(const Measure&) = delete;
  Measure(Measure&&) = delete;
  Measure& operator=(const Measure&) = delete;
  Measure& operator=(Measure&&) = delete;
  virtual ~Measure() = default;

  virtual std::string name() = 0;
  virtual std::vector<OSArgument> arguments(const Model& model) const;
  virtual bool run(Model& model, Runner& runner, const OSArgumentMap& user_arguments) const;
};

}  // Namespace openstudio

#endif  // ifndef MEASURE_HPP
