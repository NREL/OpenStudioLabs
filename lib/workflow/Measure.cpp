#include "Measure.hpp"

namespace openstudio {
bool Measure::run(Runner& runner) {
  return run_impl(runner);
}
}  // namespace openstudio
