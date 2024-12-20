#include "PythonEngine.hpp"

#include "Paths.hpp"

#include <string>
#include <stdexcept>

#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#include <Python.h>

#include "OpenStudioPYTHON_wrap.cxx"
// #include "embedded_files.hxx"

#ifdef __GNUC__
#  pragma GCC diagnostic pop
#endif

#include <fmt/format.h>

namespace openstudio {

void addToPythonPath(const openstudio::path& includePath) {

  if (includePath.empty()) {
    return;
  }

  // fmt::print("Prepending '{}' to sys.path\n", includePath);

  PyObject* unicodeIncludePath = nullptr;
  if constexpr (std::is_same_v<typename openstudio::path::value_type, wchar_t>) {
    const std::wstring ws = includePath.generic_wstring();
    unicodeIncludePath = PyUnicode_FromWideChar(ws.c_str(), static_cast<Py_ssize_t>(ws.size()));  // New reference
  } else {
    const std::string s = includePath.generic_string();
    unicodeIncludePath = PyUnicode_FromString(s.c_str());  // New reference
  }

  if (unicodeIncludePath == nullptr) {
    throw std::runtime_error(fmt::format("Unable to convert path '{}' for addition to sys.path in Python", includePath.generic_string()));
  }

  PyObject* sysPath = PySys_GetObject("path");  // Borrowed reference
  int ret = PyList_Insert(sysPath, 0, unicodeIncludePath);
  Py_DECREF(unicodeIncludePath);
  if (ret != 0) {
    throw std::runtime_error(fmt::format("Unable to add path '{}' to the sys.path in Python", includePath.generic_string()));
  }
}

void PythonEngine::pyimport(const std::string& importName, const std::string& includePath) {
  addToPythonPath(includePath);
  PyImport_ImportModule(importName.c_str());
}

PythonEngine::PythonEngine([[maybe_unused]] int argc, char* argv[]) : ScriptEngine(argc, argv), program(Py_DecodeLocale("python3.12", nullptr)) {
  // PyImport_AppendInittab("_pythonbindings", SWIG_init);

  Py_SetProgramName(program);  // optional but recommended

  Py_Initialize();

  PyObject* m = PyImport_AddModule("__main__");
  if (m == nullptr) {
    throw std::runtime_error("Unable to add module __main__ for python script execution");
  }
  m_globalDict = PyModule_GetDict(m);

  importOpenStudio();
}

void PythonEngine::importOpenStudio() {
#if defined(__APPLE__)
  // RTLD_LOCAL is import an Apple so that that Python and Ruby do not conflict
  const std::string set_dlflags_cmd = R"(
import sys
import os
pre_os_dl_open_flags = sys.getdlopenflags()
sys.setdlopenflags(os.RTLD_LOCAL)
  )";

  exec(set_dlflags_cmd.c_str());
#endif

  // generic_string() converts to a POSIX path, with forward slashes, so that pyimport doesn't choke on backslashes understood as escape sequence
  const auto bindingsDir = getCurrentModuleDir();  // / "../python";
  fmt::print("bindingsDir={}\n", bindingsDir.generic_string());
  pyimport("openstudio", bindingsDir.generic_string());
  // Somehow that doesn't suffice to register it...
  exec("import openstudio");

#if defined(__APPLE__)
  // Reset the dlopen flags to the value prior to importOpenStudio
  const std::string reset_dlflags_cmd = R"(
sys.setdlopenflags(pre_os_dl_open_flags)
  )";

  exec(reset_dlflags_cmd.c_str());
#endif
}

PythonEngine::~PythonEngine() {
  if (Py_FinalizeEx() < 0) {
    exit(120);
  }
  PyMem_RawFree(program);
}

struct PythonObject
{
  PythonObject() = default;

  explicit PythonObject(PyObject* obj) noexcept : obj_(obj) {
    if (obj_) {
      Py_INCREF(obj_);
    }
  }

  PythonObject(const PythonObject& other) noexcept : obj_(other.obj_) {
    if (obj_) {
      Py_INCREF(obj_);
    }
  }

  PythonObject(PythonObject&& other) noexcept : obj_(other.obj_) {
    // no reason to inc/dec, we just stole the ref counted object
    // from other
    other.obj_ = nullptr;
  }

  PythonObject& operator=(const PythonObject& rhs) noexcept {
    if (&rhs != this) {
      obj_ = rhs.obj_;

      if (obj_) {
        Py_INCREF(obj_);
      }
    }

    return *this;
  }

  PythonObject& operator=(PythonObject&& rhs) noexcept {
    if (&rhs != this) {
      obj_ = rhs.obj_;
      rhs.obj_ = nullptr;
    }

    return *this;
  }

  ~PythonObject() {
    if (obj_) {
      Py_DECREF(obj_);
    }
  }

  PyObject* obj_ = nullptr;
};

void PythonEngine::exec(std::string_view sv) {
  std::string command{sv};

  // PyObject* globalDict = PyModule_GetDict(m);
  //std::string fileContent = embedded_files::getFileAsString(":/python/openstudio.py");
  //PyObject* builtins = PyEval_GetBuiltins();
  //PyObject* compile = PyDict_GetItemString(builtins, "compile");
  //PyObject* code = PyObject_CallFunction(compile, "sss", fileContent.c_str(), "openstudio.py", "exec");
  //PyObject* pyModule = PyImport_ExecCodeModule("openstudio", code);

  PyObject* v = PyRun_String(command.c_str(), Py_file_input, m_globalDict, m_globalDict);
  if (v == nullptr) {
    PyErr_Print();
    throw std::runtime_error("Error executing Python code");
  }

  Py_DECREF(v);
  // Py_DecRef(pyModule);
}

ScriptObject PythonEngine::eval(std::string_view sv) {
  std::string command{sv};

  PyObject* v = PyRun_String(command.c_str(), Py_eval_input, m_globalDict, m_globalDict);
  if (v == nullptr) {
    PyErr_Print();
    throw std::runtime_error("Error executing Python code");
  }

  //share in ownership
  PythonObject return_value(v);

  //decref count returned from Python
  Py_DECREF(v);

  return ScriptObject{return_value};
}

// convert the underlying object to the correct type, then return it as a void *
// so the above template function can provide it back to the caller.
void* PythonEngine::getAs_impl(ScriptObject& obj, const std::type_info& ti) {

  auto val = std::any_cast<PythonObject>(obj.object);

  const auto& type_name = getRegisteredTypeName(ti);

  void* return_value = nullptr;

  auto* type = SWIG_Python_TypeQuery(type_name.c_str());

  if (!type) {
    throw std::runtime_error("Unable to find type in SWIG");
  }

  const auto result = SWIG_Python_ConvertPtr(val.obj_, &return_value, type, 0);

  if (!SWIG_IsOK(result)) {
    throw std::runtime_error("Error getting object from SWIG/Python");
  }

  return return_value;
}
}  // namespace openstudio

extern "C"
{
  openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]) {
    return new openstudio::PythonEngine(argc, argv);
  }
}
