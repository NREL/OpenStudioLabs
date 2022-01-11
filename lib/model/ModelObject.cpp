#include "ModelObject.hpp"
#include "Model.hpp"
#include <fmt/format.h>

namespace Test {
  class ModelObject::Impl {
   public:
    const std::string& getName() const {
      return name;
    }

    bool setName(const std::string& t_name) {
      name = t_name;
      return true;
    }

    std::string toString() const {
      constexpr auto body =
  R"EOF(  {{
    "handle": {},
    "name": {}
  }})EOF";

      return fmt::format(body, getName(), getName());
    }

   private:
    std::string handle;
    std::string name{"Default Name"};
  };

  ModelObject::ModelObject(Model & model)
    : impl{std::make_shared<Impl>()}
  {
    model.addModelObject(*this);
  }

  const std::string& ModelObject::getName() const {
    return impl->getName();
  }

  bool ModelObject::setName(const std::string& name) {
    return impl->setName(name);
  }

  std::string ModelObject::toString() const {
    return impl->toString();
  }
}  // namespace Test
