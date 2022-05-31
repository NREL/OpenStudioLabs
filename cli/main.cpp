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
  std::cout << "Hello from C++" << std::endl;

  // Load and construct the PYTHON engine on demand using dynamic loading (e.g. dlopen)
  openstudio::util::DynamicLibrary pythonEngineLib(openstudio::getCurrentModuleDir() / "libpythonengine.so");
  const std::function<ScriptEngineFactoryType> pythonFactory = pythonEngineLib.load_symbol<ScriptEngineFactoryType>("makeScriptEngine");
  const std::unique_ptr<openstudio::ScriptEngine> pythonEngine(pythonFactory(argc, argv));
  // Python works!
  pythonEngine->exec(R"(print("Hello From Python"))");

  // Load and construct the RUBY engine on demand using dynamic loading (e.g. dlopen)
  openstudio::util::DynamicLibrary rubyEngineLib(openstudio::getCurrentModuleDir() / "librubyengine.so");
  const std::function<ScriptEngineFactoryType> rubyFactory = rubyEngineLib.load_symbol<ScriptEngineFactoryType>("makeScriptEngine");
  const std::unique_ptr<openstudio::ScriptEngine> rubyEngine(rubyFactory(argc, argv));
  // Ruby works too!
  rubyEngine->exec(R"(puts("Hello from Ruby"))");

  // Boilerplate to load a prototypical PYTHON Measure.
  // This will be abstracted into a function (e.g. loadMeasure(...))
  const auto pythonMeasurePath = sourceDir() / "python/measures";
  pythonEngine->exec(fmt::format("import sys\nsys.path.append('{}')", pythonMeasurePath.string()));
  pythonEngine->exec("import test_measure");
  auto python_measure_pointer = pythonEngine->eval("test_measure.PythonTestMeasure()");
  auto python_measure = pythonEngine->getAs<openstudio::Measure*>(python_measure_pointer);
  std::cout << "\nPython measure name: " << python_measure->name() << std::endl;

  // Boilerplate to load a prototypical RUBY Measure.
  const auto rubyMeasurePath = sourceDir() / "ruby/measures/test_measure.rb";
  rubyEngine->exec(fmt::format("require '{}'", rubyMeasurePath.string()));
  auto ruby_measure_pointer = rubyEngine->eval("RubyTestMeasure.new()");
  auto ruby_measure = rubyEngine->getAs<openstudio::Measure*>(ruby_measure_pointer);
  std::cout << "Ruby measure name: " << ruby_measure->name() << std::endl;

  // Run a mock OpenStudio Workflow
  // In the finished product this is orchestrated by the osw input
  openstudio::Model m;
  std::cout << "\nLook. The starting model is empty" << std::endl;
  std::cout << m.toString() << std::endl << std::endl;

  openstudio::ModelObject obj(m);
  obj.setName("ModelObject created in C++");

  openstudio::Runner r(m);
  python_measure->run(r);
  ruby_measure->run(r);

  std::cout << "Now the model is populated from C++, Python, and Ruby" << std::endl;
  std::cout << m.toString() << std::endl;
}

