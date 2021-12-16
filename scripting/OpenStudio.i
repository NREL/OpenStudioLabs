#ifndef WORKFLOW_I
#define WORKFLOW_I

#include <stl.i>

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

%module mylib
%module(directors="1") mylib

%include <stl.i>
%include <std_string.i>
%include <std_vector.i>

%feature("director") Measure;

#if defined(SWIGPYTHON)
%rename (PythonMeasure) Test::Measure;
#endif

%{
  #include <lib/model/ModelObject.hpp>
  #include <lib/model/Model.hpp>
  #include <lib/workflow/Runner.hpp>
  #include <lib/workflow/Measure.hpp>
%}

%include <lib/model/ModelObject.hpp>
%include <lib/model/Model.hpp>
%include <lib/workflow/Runner.hpp>
%include <lib/workflow/Measure.hpp>

#endif //WORKFLOW_I
