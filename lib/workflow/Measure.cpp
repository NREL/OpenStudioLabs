#include "Measure.hpp"
#include "OSArgument.hpp"

#include <fmt/format.h>

namespace openstudio {

std::vector<OSArgument> Measure::arguments(const openstudio::Model& /*model*/) const {
  return {};
}

bool Measure::run(Model& /* model */, Runner& /*runner*/, const OSArgumentMap& /*user_arguments*/) const {
  fmt::print("Base class Measure::run called");
  return true;
}

bool PythonMeasure::run(Model& /* model */, Runner& /*runner*/, const OSArgumentMap& /*user_arguments*/) const {
  fmt::print("Base class PythonMeasure::run called");
  return true;
}

}  // namespace openstudio
