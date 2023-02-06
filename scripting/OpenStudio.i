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

%module openstudio
%module(directors="1") openstudio

%include <stl.i>
%include <std_string.i>
%include <std_vector.i>

%feature("director") Measure;

%{
  #include <ModelObject.hpp>
  #include <Model.hpp>
  #include <Runner.hpp>
  #include <Measure.hpp>
  #include <PyRbyTypes.hpp>
%}

%include <ModelObject.hpp>
%include <Model.hpp>
%include <Runner.hpp>
%include <Measure.hpp>
%include <PyRbyTypes.hpp>

%template(BarVector) std::vector<Bar>;

#endif //WORKFLOW_I
