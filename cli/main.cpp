// CMake defines WITHPYTHON / WITHRUBY (or not)

#include <iostream>
#include <cassert>
#include <functional>
#include <fmt/format.h>

#include "scripting/ScriptEngine.hpp"
#include "Measure.hpp"
#include "SpecialRunner.hpp"
#include "Model.hpp"
#include "DynamicLibrary.hpp"
#include "Paths.hpp"
#include "config.hxx"

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char* argv[]) {
  openstudio::util::DynamicLibrary pythonEngineLib(openstudio::getCurrentModuleDir() / "libpythonengine.so");
  std::function<ScriptEngineFactoryType> pythonFactory = pythonEngineLib.load_symbol<ScriptEngineFactoryType>("makeScriptEngine");
  std::unique_ptr<openstudio::ScriptEngine> pythonEngine(pythonFactory(argc, argv));

  pythonEngine->exec(R"(print("Hello From Python"))");

  openstudio::util::DynamicLibrary rubyEngineLib(openstudio::getCurrentModuleDir() / "librubyengine.so");
  std::function<ScriptEngineFactoryType> rubyFactory = rubyEngineLib.load_symbol<ScriptEngineFactoryType>("makeScriptEngine");
  std::unique_ptr<openstudio::ScriptEngine> rubyEngine(rubyFactory(argc, argv));

  rubyEngine->exec(R"(puts("Hello from Ruby"))");

  rubyEngine->registerType<Test::Measure*>("Test::Measure *");
  pythonEngine->registerType<Test::Measure*>("Test::Measure *");

  const auto rubyMeasurePath = sourceDir() / "ruby/measures/test_measure.rb";
  rubyEngine->exec(fmt::format("require '{}'", rubyMeasurePath.string()));
  auto ruby_measure = rubyEngine->eval("RubyTestMeasure.new()");
  auto* ruby_measure_from_cpp = rubyEngine->getAs<Test::Measure*>(ruby_measure);
  assert(ruby_measure_from_cpp);
  std::cout << "Ruby measure name: " << ruby_measure_from_cpp->name() << '\n';

  const auto pythonMeasurePath = sourceDir() / "python/measures";
  pythonEngine->exec(fmt::format("import sys\nsys.path.append('{}')", pythonMeasurePath.string()));
  pythonEngine->exec("import test_measure");
  auto python_measure = pythonEngine->eval("test_measure.PythonTestMeasure()");
  auto* python_measure_from_cpp = pythonEngine->getAs<Test::Measure*>(python_measure);
  assert(python_measure_from_cpp);

  std::cout << "Python measure name: " << python_measure_from_cpp->name() << '\n';

  Test::Model m;

  Test::ModelObject obj(m);
  obj.setName("ModelObject created in C++");

  Test::SpecialRunner sr(m);
  ruby_measure_from_cpp->run(sr);
  python_measure_from_cpp->run(sr);

  std::cout << m.toString() << std::endl;
}

