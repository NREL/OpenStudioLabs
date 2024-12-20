#ifndef PYTHONENGINE_included
#define PYTHONENGINE_included

#include <memory>

#include "../../scripting/ScriptEngine.hpp"
#include "../../scripting/ScriptEngineAPI.hpp"

#ifndef PyObject_HEAD
struct _object;
using PyObject = _object;
#endif

namespace openstudio {
class PythonEngine final : public ScriptEngine
{
 public:
  PythonEngine(int argc, char* argv[]);
  ~PythonEngine() override;

  PythonEngine(const PythonEngine&) = delete;
  PythonEngine(PythonEngine&&) = delete;
  PythonEngine& operator=(const PythonEngine&) = delete;
  PythonEngine& operator=(PythonEngine&&) = delete;

  ScriptObject eval(std::string_view sv) override;
  void exec(std::string_view sv) override;

 protected:
  // convert the underlying object to the correct type, then return it as a void *
  // so the above template function can provide it back to the caller.
  void* getAs_impl(ScriptObject& obj, const std::type_info&) override;
  void importOpenStudio();
  void pyimport(const std::string& importName, const std::string& includePath);

 private:
  wchar_t* program;
  PyObject* m_globalDict;
};

}  // namespace openstudio

extern "C"
{
  SCRIPTENGINE_API openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]);
}

#endif
