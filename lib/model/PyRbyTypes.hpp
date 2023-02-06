#include <vector>
#include <string>
//#include "UUID.hpp"
//#include "../../model/Model.hpp"

struct Bar {
  std::string name() const;
};

struct Foo {
  std::vector<Bar> bars() const; 

  //std::vector<openstudio::UUID> ids() const;

  //std::vector<openstudio::model::ModelObject> modelObjects() const;

  //openstudio::model::Model m;
};
