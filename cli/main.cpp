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
#include "config.hxx"

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char* argv[]) {
  
  openstudio::util::DynamicLibrary pythonEngineLib("/home/kbenne/Development/Cpp_Swig_Ruby_Python_MCVE/build/Products/libpythonengine.so");
  std::function<ScriptEngineFactoryType> pythonFactory = pythonEngineLib.load_symbol<ScriptEngineFactoryType>("makeScriptEngine");
  std::unique_ptr<openstudio::ScriptEngine> pythonEngine(pythonFactory(argc, argv));

  pythonEngine->exec(R"(print("Hello From Python"))");

  openstudio::util::DynamicLibrary rubyEngineLib("/home/kbenne/Development/Cpp_Swig_Ruby_Python_MCVE/build/Products/librubyengine.so");
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

  //auto printObjectNames = [](Test::Model& m) {
  //  for (int i = 0; i < m.numObjects(); ++i) {
  //    std::cout << " * " << i <<" = " << m.getObject(i).getName() << '\n';
  //  }
  //};

  Test::Model m{"C++ Model"};
  m.pushObject("C++ object");

  Test::SpecialRunner sr(m);

  //printObjectNames(sr.get_current_model());

  ruby_measure_from_cpp->run(sr);
  python_measure_from_cpp->run(sr);


  //std::cout << "C++: " <<  sr.get_current_model().numObjects() << " objects\n";
  //printObjectNames(sr.get_current_model());


  /////////---------//#if defined(WITHPYTHON) && defined(WITHRUBY)
  /////////---------std::cout << "After Running Ruby and Python: model is named " << sr.get_current_model().getName() << '\n';
  /////////---------//#elif defined (WITHRUBY)
  /////////---------//  std::cout << "After Running Ruby only: model is named " << sr.get_current_model().getName() << '\n';
  /////////---------//#elif defined(WITHPYTHON)
  /////////---------//  std::cout << "After Running Python only: model is named " << sr.get_current_model().getName() << '\n';
  /////////---------//#endif

  /////////---------for (const auto& op : sr.get_current_model().opsPerformed()) {
  /////////---------  std::cout << "Op 'run' from script: " << op << '\n';
  /////////---------}

  /////////---------std::cout << "C++: " <<  sr.get_current_model().numObjects() << " objects\n";
  /////////---------printObjectNames(sr.get_current_model());
}

