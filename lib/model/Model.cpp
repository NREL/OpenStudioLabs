#include "Model.hpp"
#include "ModelObject.hpp"
#include <sstream>
#include <iostream>

namespace openstudio {
  class Model::Impl {
   public:
    void addModelObject(ModelObject modelObject) {
      objects.push_back(modelObject);
    }

    std::string toString() const {
      std::stringstream ss;

      ss << "{" << std::endl;
      for (const auto &mo: objects) {
        ss << mo.toString() << "," << std::endl;
      }
      ss << "}" << std::endl;

      return ss.str();
    }

   private:
    std::vector<ModelObject> objects;
  };

  Model::Model()
    : impl{std::make_shared<Impl>()}
  {
  }

  void Model::addModelObject(ModelObject modelObject) {
    impl->addModelObject(modelObject);
  }

  std::string Model::toString() const {
    return impl->toString();
  }
}  // namespace openstudio
