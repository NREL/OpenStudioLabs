#include <iostream>
#include <fmt/format.h>
#include "ScriptEngine.hpp"
#include "Measure.hpp"
#include "Runner.hpp"
#include "Model.hpp"
#include "DynamicLibrary.hpp"
#include "Paths.hpp"
#include "config.hxx"

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char* argv[]) {
  fmt::print("Hello from C++\n");

  std::string engines = "C++ driver";

  fmt::print("\n\n========== Initialization of engines ==========\n");
#if USE_RUBY_ENGINE
  engines += ", RubyEngine";
  fmt::print("Initializing the RubyEngine\n");
  // Load and construct the RUBY engine on demand using dynamic loading (e.g. dlopen)
  openstudio::util::DynamicLibrary rubyEngineLib(openstudio::getCurrentModuleDir() / getSharedModuleName("rubyengine"));
  const std::function<ScriptEngineFactoryType> rubyFactory = rubyEngineLib.load_symbol<ScriptEngineFactoryType>("makeScriptEngine");
  const std::unique_ptr<openstudio::ScriptEngine> rubyEngine(rubyFactory(argc, argv));
  rubyEngine->registerType<openstudio::Measure*>("openstudio::Measure *");

  // Ruby works too!
  rubyEngine->exec(R"(puts("Hello from Ruby"))");
#endif

#if USE_PYTHON_ENGINE
  engines += ", PythonEngine";
  fmt::print("Initializing the PythonEngine\n");
  // Load and construct the PYTHON engine on demand using dynamic loading (e.g. dlopen)
  openstudio::util::DynamicLibrary pythonEngineLib(openstudio::getCurrentModuleDir() / getSharedModuleName("pythonengine"));
  const std::function<ScriptEngineFactoryType> pythonFactory = pythonEngineLib.load_symbol<ScriptEngineFactoryType>("makeScriptEngine");
  const std::unique_ptr<openstudio::ScriptEngine> pythonEngine(pythonFactory(argc, argv));
  pythonEngine->registerType<openstudio::Measure*>("openstudio::Measure *");
  // Python works!
  pythonEngine->exec(R"(print("Hello From Python"))");
#endif

  fmt::print("\n\n========== Loading Measures ==========\n");

#if USE_RUBY_ENGINE
  // Boilerplate to load a prototypical RUBY Measure.
  const auto rubyMeasurePath = sourceDir() / "ruby/measures/test_measure.rb";
  rubyEngine->exec(fmt::format("require '{}'", rubyMeasurePath.string()));
  auto ruby_measure_pointer = rubyEngine->eval("RubyTestMeasure.new()");
  auto* ruby_measure = rubyEngine->getAs<openstudio::Measure*>(ruby_measure_pointer);
  fmt::print("Ruby measure name: {}\n", ruby_measure->name());
#endif

#if USE_PYTHON_ENGINE
  // Boilerplate to load a prototypical PYTHON Measure.
  // This will be abstracted into a function (e.g. loadMeasure(...))
  const auto pythonMeasurePath = sourceDir() / "python/measures";
  pythonEngine->exec(fmt::format("import sys\nsys.path.append('{}')", pythonMeasurePath.string()));
  pythonEngine->exec("import test_measure");
  auto python_measure_pointer = pythonEngine->eval("test_measure.PythonTestMeasure()");
  auto* python_measure = pythonEngine->getAs<openstudio::Measure*>(python_measure_pointer);
  fmt::print("\nPython measure name: {}\n", python_measure->name());
#endif

  fmt::print("\n\n========== Running Measures ==========\n");

  // Run a mock OpenStudio Workflow
  // In the finished product this is orchestrated by the osw input
  openstudio::Model m;
  fmt::print("Look. The starting model is empty:\n-----\n{}\n-----\n", m.toString());

  openstudio::ModelObject obj(m);
  obj.setName("ModelObject created in C++");

  openstudio::Runner runner(m);

#if USE_RUBY_ENGINE
  fmt::print("ruby_measure->run\n");
  ruby_measure->run(runner);
#endif

#if USE_PYTHON_ENGINE
  fmt::print("python_measure->run\n");
  python_measure->run(runner);
#endif

  fmt::print("Now the model is populated from {}\n", engines);
  fmt::print("\n-----\n{}\n-----\n", m.toString());
}
