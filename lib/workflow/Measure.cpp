#include "Measure.hpp"

#include <fmt/format.h>

namespace openstudio {

bool Measure::run(Model& /* model */, Runner& /*runner*/, const OSArgumentMap& /*user_arguments*/) const {
  fmt::print("Base class Measure::run called");
  return true;
}

}  // namespace openstudio
