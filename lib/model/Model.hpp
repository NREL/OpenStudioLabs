#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>
#include "ModelObject.hpp"

namespace Test {
  class Model
  {
    public:
      explicit Model();
      std::string toString() const;

    protected:
      friend class ModelObject;

      void addModelObject(ModelObject modelObject);

    private:
      class Impl;
      std::shared_ptr<Impl> impl;
  };
}  // namespace Test

#endif
