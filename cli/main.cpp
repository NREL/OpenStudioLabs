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
  
  openstudio::util::DynamicLibrary pythonEngineLib("libpythonengine.so");
  std::function<ScriptEngineFactoryType> factory = pythonEngineLib.load_symbol<ScriptEngineFactoryType>("makeScriptEngine");
  std::unique_ptr<openstudio::ScriptEngine> pythonEngine(factory(argc, argv));

  //#ifdef WITHRUBY
  //ruby.exec(R"(puts("Hello from Ruby"))");
  //#endif

  pythonEngine->exec(R"(print("Hello From Python"))");

  //#ifdef WITHRUBY
  //ruby.registerType<Test::Measure*>("Test::Measure *");
  //#endif

  pythonEngine->registerType<Test::Measure*>("Test::Measure *");

  //#ifdef WITHRUBY
  //const auto rubyMeasurePath = sourceDir() / "ruby/test_measure.rb";
  //ruby.exec(fmt::format("require '{}'", rubyMeasurePath.string()));
  //auto ruby_measure = ruby.eval("RubyTestMeasure.new()");
  //auto* ruby_measure_from_cpp = ruby.getAs<Test::Measure*>(ruby_measure);
  //assert(ruby_measure_from_cpp);
  //std::cout << "Ruby measure name: " << ruby_measure_from_cpp->name() << '\n';
  //#endif

  const auto pythonMeasurePath = sourceDir() / "python/measures";
  pythonEngine->exec(fmt::format("import sys\nsys.path.append('{}')\nprint(f'{{sys.path}}')", pythonMeasurePath.string()));
  pythonEngine->exec("import test_measure");
  auto python_measure = pythonEngine->eval("test_measure.PythonTestMeasure()");
  auto* python_measure_from_cpp = pythonEngine->getAs<Test::Measure*>(python_measure);
  assert(python_measure_from_cpp);

  std::cout << "Python measure name: " << python_measure_from_cpp->name() << '\n';

  auto printObjectNames = [](Test::Model& m) {
    for (int i = 0; i < m.numObjects(); ++i) {
      std::cout << " * " << i <<" = " << m.getObject(i).getName() << '\n';
    }
  };

  Test::Model m{"C++ Model"};
  m.pushObject("C++ object");
  Test::SpecialRunner sr(m);
  std::cout << "Starting out with a Model in C++ called: " << sr.get_current_model().getName() << '\n';
  std::cout << "C++: starting with " <<  sr.get_current_model().numObjects() << " objects\n";
  printObjectNames(sr.get_current_model());

  //#ifdef WITHRUBY
  //  std::cout << "\n\n========== START RUBY ==========\n";
  //  ruby_measure_from_cpp->run(sr);
  //  std::cout << "========== FINISHED RUBY ==========\n\n";
  //#else
  //  std::cout << "\n\n########## RUBY ISN'T ENABLED ##########\n\n";
  //#endif


  std::cout << "C++: " <<  sr.get_current_model().numObjects() << " objects\n";
  printObjectNames(sr.get_current_model());

  std::cout << "\n\n========== START PYTHON ==========\n";
  python_measure_from_cpp->run(sr);
  std::cout << "========== FINISHED PYTHON ==========\n\n";

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

