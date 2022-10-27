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

#if defined(SWIGPYTHON)
  // Avoid triggering a SWIG warning: 'print' is a python keyword
  %rename(toString) openstudio::OSArgument::print;
#endif

%{
  #include <ModelObject.hpp>
  #include <Model.hpp>
  #include <Runner.hpp>
  #include <Measure.hpp>
  #include <OSArgument.hpp>
%}

%feature("director") Model;
%feature("director") Measure;
%feature("director") OSRunner;

%feature("director") OSArgument;
%template(OSArgumentVector) std::vector<openstudio::OSArgument>;
%template(OSArgumentMap) std::map<std::string, openstudio::OSArgument>;

%include <ModelObject.hpp>
%include <Model.hpp>
%include <Runner.hpp>
%include <Measure.hpp>
%include <OSArgument.hpp>

#endif //WORKFLOW_I
