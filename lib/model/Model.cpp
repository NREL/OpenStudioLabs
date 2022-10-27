#include "Model.hpp"
#include "ModelObject.hpp"
#include <sstream>
#include <iostream>
#include <utility>

namespace openstudio {
class Model::Impl
{
 public:
  void addModelObject(ModelObject modelObject) {
    objects.emplace_back(std::move(modelObject));
  }

  std::string toString() const {
    std::stringstream ss;

    ss << "{" << std::endl;
    for (const auto& mo : objects) {
      ss << mo.toString() << "," << std::endl;
    }
    ss << "}" << std::endl;

    return ss.str();
  }

 private:
  std::vector<ModelObject> objects;
};

Model::Model() : impl{std::make_shared<Impl>()} {}

void Model::addModelObject(ModelObject modelObject) {
  impl->addModelObject(std::move(modelObject));
}

std::string Model::toString() const {
  return impl->toString();
}
}  // namespace openstudio
