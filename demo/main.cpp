#include "ScriptEngine.hpp"
#include "DynamicLibrary.hpp"
#include "Paths.hpp"
#include "config.hxx"

int main([[maybe_unused]] const int argc, [[maybe_unused]] const char* argv[]) {
  std::vector<std::string> args(argv, argv + argc);
  args.erase(args.begin());

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

  const auto pythonScript = R"(
import openstudio
f = openstudio.Foo();
bars = f.bars()
for bar in bars:
  print(bar.name())
)";

  pythonEngine->exec(pythonScript);

const auto rubyScript = R"(
  f = Openstudio::Foo.new();
  b = f.bars()
  b.each do |bar|
    puts bar.name()
  end
)";

rubyEngine->exec(rubyScript);

  return 0;
}
