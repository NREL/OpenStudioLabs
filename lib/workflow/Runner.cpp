#include "Runner.hpp"
#include "OSArgument.hpp"

#include <sstream>
#include <stdexcept>

namespace openstudio {

double Runner::getDoubleArgumentValue(const std::string& argument_name, const std::map<std::string, openstudio::OSArgument>& user_arguments) {
  std::stringstream ss;

  auto it = user_arguments.find(argument_name);
  if (it != user_arguments.end()) {
    if (it->second.hasValue()) {
      return it->second.valueAsDouble();
    }
  }

  ss << "No value found for argument '" << argument_name << "'.";
  if (it != user_arguments.end()) {
    ss << " Full argument as passed in by user:" << '\n' << it->second;
  }
  throw std::runtime_error(ss.str());
  // return 0.0;
}

}  // namespace openstudio
