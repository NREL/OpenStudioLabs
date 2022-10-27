#ifndef OPENSTUDIO_SWIG_RUNNER_HPP
#define OPENSTUDIO_SWIG_RUNNER_HPP

#include "../model/Model.hpp"

#include <optional>
#include <map>

namespace openstudio {

class OSArgument;

class Runner
{
 public:
  virtual ~Runner() = default;
  Runner& operator=(Runner&&) = delete;
  Runner& operator=(const Runner&) = delete;
  Runner(Runner&&) = delete;
  Runner(const Runner&) = delete;
  Runner() = delete;
  Runner(Model model) : model_(std::move(model)) {}

  double getDoubleArgumentValue(const std::string& argument_name, const std::map<std::string, OSArgument>& user_arguments);

 private:
  std::optional<Model> model_;
};

}  // namespace openstudio

#endif  //OPENSTUDIO_SWIG_RUNNER_HPP
