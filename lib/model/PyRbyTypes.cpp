#include "PyRbyTypes.hpp"
//#include "../../model/FanConstantVolume.hpp"

std::string Bar::name() const {
  return "abar";
}

std::vector<Bar> Foo::bars() const {
  return {Bar(), Bar(), Bar()};
}

//std::vector<openstudio::UUID> Foo::ids() const {
//  return {
//    openstudio::createUUID(),
//    openstudio::createUUID(),
//    openstudio::createUUID()};
//}
//
//std::vector<openstudio::model::ModelObject> Foo::modelObjects() const {
//  return {
//    openstudio::model::FanConstantVolume(m),
//    openstudio::model::FanConstantVolume(m),
//    openstudio::model::FanConstantVolume(m)
//  };
//}

