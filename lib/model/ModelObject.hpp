#ifndef MODELOBJECT_HPP
#define MODELOBJECT_HPP

#include <string>
#include <memory>

namespace openstudio {

class Model;

class ModelObject
{
 public:
  explicit ModelObject(Model& model);

  std::string toString() const;
  const std::string& getName() const;
  bool setName(const std::string& name);

 private:
  class Impl;
  std::shared_ptr<Impl> impl;
};

}  // namespace openstudio

#endif  // MODELOBJECT_HPP
