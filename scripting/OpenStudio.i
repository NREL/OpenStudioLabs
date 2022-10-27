#ifndef WORKFLOW_I
#define WORKFLOW_I

#include <stl.i>

#define MEASURE_API

%begin %{
  // ... code in begin section ...
%}

%runtime %{
  // ... code in runtime section ...
%}

%header %{
  // ... code in header section ...
%}

%wrapper %{
  // ... code in wrapper section ...
%}

%init %{
  // ... code in init section ...
%}

%module openstudio
%module(directors="1") openstudio

%include <stl.i>
%include <std_map.i>
%include <std_string.i>
%include <std_vector.i>

#if defined(SWIGRUBY)
  %ignore openstudio::PythonMeasure;
#elif defined(SWIGPYTHON)
  // Avoid triggering a SWIG warning: 'print' is a python keyword
  %rename(toString) openstudio::OSArgument::print;

  %ignore openstudio::Measure;
#endif

%{
  #include <OSArgument.hpp>
  #include <ModelObject.hpp>
  #include <Model.hpp>
  #include <Runner.hpp>
  #include <Measure.hpp>
%}

#if defined(SWIGPYTHON)
  %feature("director") PythonMeasure;
  %rename (Measure) openstudio::PythonMeasure;
#else
  %feature("director") Measure;
#endif

%feature("director") Model;
%feature("director") Measure;
%feature("director") OSRunner;

%feature("director") OSArgument;
%template(OSArgumentVector) std::vector<openstudio::OSArgument>;
%template(OSArgumentMap) std::map<std::string, openstudio::OSArgument>;

%include <OSArgument.hpp>
%include <ModelObject.hpp>
%include <Model.hpp>
%include <Runner.hpp>
%include <Measure.hpp>

#endif //WORKFLOW_I
