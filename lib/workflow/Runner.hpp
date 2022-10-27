#ifndef OPENSTUDIO_SWIG_RUNNER_HPP
#define OPENSTUDIO_SWIG_RUNNER_HPP

#include "Model.hpp"

namespace openstudio {

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

  Model& get_current_model() {
    return model_;
  }

 private:
  Model model_;
};

}  // namespace openstudio

#endif  //OPENSTUDIO_SWIG_RUNNER_HPP
