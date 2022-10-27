#include "PythonEngine.hpp"
#include <string>
#include <stdexcept>

#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#include <Python.h>

#include "OpenStudioPYTHON_wrap.cxx"
#include "embedded_files.hxx"

#ifdef __GNUC__
#  pragma GCC diagnostic pop
#endif

namespace openstudio {

PythonEngine::PythonEngine([[maybe_unused]] int argc, char* argv[]) : ScriptEngine(argc, argv), program(Py_DecodeLocale("python3.7", nullptr)) {
  PyImport_AppendInittab("_pythonbindings", SWIG_init);

  Py_SetProgramName(program);  // optional but recommended

  Py_Initialize();
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

  PyObject* m = PyImport_AddModule("__main__");
  if (m == nullptr) {
    throw std::runtime_error("Unable to add module __main__ for python script execution");
  }

  PyObject* globalDict = PyModule_GetDict(m);

  std::string fileContent = embedded_files::getFileAsString(":/python/openstudio.py");

  PyObject* builtins = PyEval_GetBuiltins();
  PyObject* compile = PyDict_GetItemString(builtins, "compile");
  PyObject* code = PyObject_CallFunction(compile, "sss", fileContent.c_str(), "openstudio.py", "exec");
  PyObject* pyModule = PyImport_ExecCodeModule("openstudio", code);

  PyObject* v = PyRun_String(command.c_str(), Py_file_input, globalDict, globalDict);
  if (v == nullptr) {
    PyErr_Print();
    throw std::runtime_error("Error executing Python code");
  }

  Py_DECREF(v);
  Py_DecRef(pyModule);
}

ScriptObject PythonEngine::eval(std::string_view sv) {
  std::string command{sv};

  PyObject* m = PyImport_AddModule("__main__");
  if (m == nullptr) {
    throw std::runtime_error("Unable to add module __main__ for python script execution");
  }

  PyObject* d = PyModule_GetDict(m);

  PyObject* v = PyRun_String(command.c_str(), Py_eval_input, d, d);
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