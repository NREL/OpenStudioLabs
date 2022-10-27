/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "OSArgument.hpp"

#include <sstream>
#include <fmt/format.h>
#include <string_view>

namespace openstudio {

inline std::string ascii_to_lower_copy(std::string_view input) {
  std::string result{input};
  constexpr auto to_lower_diff = 'a' - 'A';
  for (auto& c : result) {
    if (c >= 'A' && c <= 'Z') {
      c += to_lower_diff;
    }
  }
  return result;
}

bool istringEqual(std::string_view lhs, std::string_view rhs) {
  return ascii_to_lower_copy(lhs) == ascii_to_lower_copy(rhs);
}

OSArgument OSArgument::clone() const {
  OSArgument result(*this);
  return result;
}

OSArgument OSArgument::makeBoolArgument(const std::string& name, bool required, bool modelDependent) {
  OSArgument result(name, OSArgumentType::Boolean, required, modelDependent);
  return result;
}

OSArgument OSArgument::makeDoubleArgument(const std::string& name, bool required, bool modelDependent) {
  OSArgument result(name, OSArgumentType::Double, required, modelDependent);
  return result;
}

OSArgument OSArgument::makeIntegerArgument(const std::string& name, bool required, bool modelDependent) {
  OSArgument result(name, OSArgumentType::Integer, required, modelDependent);
  return result;
}

OSArgument OSArgument::makeStringArgument(const std::string& name, bool required, bool modelDependent) {
  OSArgument result(name, OSArgumentType::String, required, modelDependent);
  return result;
}

OSArgument OSArgument::makePathArgument(const std::string& name, bool required, bool modelDependent) {
  OSArgument result(name, OSArgumentType::Path, required, modelDependent);
  return result;
}

OSArgument OSArgument::makeSeparatorArgument(const std::string& name, bool modelDependent) {
  OSArgument result(name, OSArgumentType::Separator, false, modelDependent);
  return result;
}

std::string OSArgument::name() const {
  return m_name;
}

std::string OSArgument::displayName() const {
  return m_displayName;
}

std::optional<std::string> OSArgument::description() const {
  return m_description;
}

OSArgumentType OSArgument::type() const {
  return m_type;
}

std::optional<std::string> OSArgument::units() const {
  return m_units;
}

bool OSArgument::required() const {
  return m_required;
}

bool OSArgument::modelDependent() const {
  return m_modelDependent;
}

bool OSArgument::hasValue() const {
  // If index is 0, then its std::monostate (=empty)
  return (m_value.index() != 0);
}

bool OSArgument::valueAsBool() const {
  if (!hasValue()) {
    throw std::runtime_error("This argument does not have a value set.");
  }
  if (type() != OSArgumentType::Boolean) {
    throw std::runtime_error(fmt::format("This argument is of type {}, not of type Bool.", int(type())));
  }

  // Note JM 2019-05-17: This is functionally equivalent to `std::get<bool>(m_value)` except it doesn't risk throwing
  // std::bad_variant_access which isn't available on mac prior to 10.14
  // No need to check if get_if succeeds because we checked the type above
  return *(std::get_if<bool>(&m_value));
}

double OSArgument::valueAsDouble() const {
  if (!hasValue()) {
    throw std::runtime_error("This argument does not have a value set.");
  }
  if ((type() != OSArgumentType::Double) && (type() != OSArgumentType::Integer)) {
    throw std::runtime_error(fmt::format("This argument is of type {}, not of type Double.", int(type())));
  }

  double result;
  if (type() == OSArgumentType::Double) {
    result = *(std::get_if<double>(&m_value));
  } else {
    result = *(std::get_if<int>(&m_value));
    fmt::print("Warn: This argument is of type 'Integer' but returning as a Double as requested. You should consider using valueAsInteger instead\n");
  }

  return result;
}

int OSArgument::valueAsInteger() const {
  if (!hasValue()) {
    throw std::runtime_error("This argument does not have a value set.");
  }
  if (type() != OSArgumentType::Integer) {
    throw std::runtime_error(fmt::format("This argument is of type {}, not of type Integer.", int(type())));
  }

  return *(std::get_if<int>(&m_value));
}

std::string OSArgument::valueAsString() const {
  if (!hasValue()) {
    throw std::runtime_error("This argument does not have a value set.");
  }

  return printOSArgumentVariant(m_value);
}

std::filesystem::path OSArgument::valueAsPath() const {
  if (!hasValue()) {
    throw std::runtime_error("This argument does not have a value set.");
  }
  if (type() != OSArgumentType::Path) {
    throw std::runtime_error(fmt::format("This argument is of type {}, not of type Path.", int(type())));
  }

  return *(std::get_if<std::filesystem::path>(&m_value));
}

void OSArgument::setDisplayName(const std::string& displayName) {
  m_displayName = displayName;
  onChange();
}

void OSArgument::setDescription(const std::string& description) {
  m_description = description;
  onChange();
}

void OSArgument::setUnits(const std::string& units) {
  m_units = units;
  onChange();
}

bool OSArgument::setValue(bool value) {
  bool result = false;
  if (m_type == OSArgumentType::Boolean) {
    m_value = value;
    onChange();
    result = true;
  }
  return result;
}

bool OSArgument::setValue(double value) {
  bool result = false;
  if (m_type == OSArgumentType::Double) {
    m_value = value;
    onChange();
    result = true;

  } else if (m_type == OSArgumentType::Integer) {
    // Let a double be assigned to an int, only if said double is really an integer expressed as a double
    int test = (int)floor(value);
    if (test == value) {
      result = setValue(test);
    } else {
      result = false;
    }
  }

  return result;
}

bool OSArgument::setValue(int value) {
  bool result = false;
  if (m_type == OSArgumentType::Integer) {
    m_value = value;
    result = true;

  } else if (m_type == OSArgumentType::Double) {
    // If we pass an int, but our type is double, we let it happen
    m_value.emplace<double>(value);
    result = true;
  }

  if (result) {
    onChange();
  }

  return result;
}

bool OSArgument::setValue(const std::filesystem::path& value) {
  bool result = false;
  if (m_type == OSArgumentType::Path) {
    m_value = value;
    onChange();
    result = true;
  }
  return result;
}

bool OSArgument::setValue(const char* value) {
  return setValue(std::string(value));
}

bool OSArgument::setValue(const std::string& value) {
  bool result = setStringInternal(m_value, value);
  if (result) {
    onChange();
  }
  return result;
}

void OSArgument::clearValue() {
  m_value = OSArgumentVariant();
  onChange();
}

bool OSArgument::setStringInternal(OSArgumentVariant& variant, const std::string& value) {
  bool result = false;

  if (m_type == OSArgumentType::Boolean) {
    if (istringEqual(value, "true")) {
      variant = true;
      result = true;
    } else if (istringEqual(value, "false")) {
      variant = false;
      result = true;
    } else {
      fmt::print("Debug: Unknown value '{}' for argument of type Bool.", value);
    }
  } else if (m_type == OSArgumentType::Double) {
    try {
      auto const double_val = std::stod(value, nullptr);
      variant = double_val;
      result = true;
    } catch (std::exception&) {
      fmt::print("Debug: Unknown value '{}' for argument of type Double.", value);
    }
  } else if (m_type == OSArgumentType::Integer) {
    try {
      auto const int_val = std::stoi(value, nullptr);
      variant = int_val;
      result = true;
    } catch (std::exception&) {
      fmt::print("Debug: Unknown value '{}' for argument of type Integer.", value);
    }
  } else if (m_type == OSArgumentType::String) {
    variant = value;
    result = true;
  } else if (m_type == OSArgumentType::Path) {
    std::filesystem::path temp = std::filesystem::path(value);
    variant = temp;
    result = true;
  }

  // Don't really need to handle the other case, as it can only be a separator

  return result;
}

std::string OSArgument::print() const {
  std::stringstream ss;

  // name
  ss << name();
  if (!displayName().empty()) {
    ss << " (" << displayName() << ")";
  }
  ss << '\n';

  // type and required
  ss << int(type()) << ", ";
  if (required()) {
    ss << "Required";
  } else {
    ss << "Optional";
  }
  ss << '\n';

  // value
  ss << "Value: ";
  if (hasValue()) {
    ss << printValue() << " ";
  }
  ss << '\n';

  return ss.str();
}

std::string OSArgument::printValue() const {
  std::string result;

  OSArgumentVariant toPrint;
  if (hasValue()) {
    toPrint = m_value;
  }

  // If not std::monostate (= empty)
  if (toPrint.index() != 0) {
    result = printOSArgumentVariant(toPrint);
  }

  return result;
}

// ostream operators for OSArgument
std::ostream& operator<<(std::ostream& os, const OSArgument& arg) {
  os << arg.print();
  return os;
}

std::string OSArgument::printOSArgumentVariant(const OSArgumentVariant& toPrint) const {
  std::stringstream ss;

  // We use std::visit, filtering out the case where it's monostate
  // Aside from monostate, every possible type is streamable
  //std::visit(
  //[&ss](const auto& val){
  ////Needed to properly compare the types
  //using T = std::remove_cv_t<std::remove_reference_t<decltype(val)>>;
  //if constexpr (!std::is_same_v<T, std::monostate>) {
  //ss << val;
  //}
  //},
  //arg);

  // Note JM 2019-05-17: std::visit is problematic on mac below 10.14, because it might throw std::bad_variant_access
  // So we don't use it here. Same with std::get, so we use get_if instead
  if (const auto* p = std::get_if<bool>(&toPrint)) {
    ss << std::boolalpha << *p;
  } else if (const auto* p = std::get_if<double>(&toPrint)) {
    ss << *p;
  } else if (const auto* p = std::get_if<int>(&toPrint)) {
    ss << *p;
  } else if (const auto* p = std::get_if<std::string>(&toPrint)) {
    ss << *p;
  } else if (const auto* p = std::get_if<std::filesystem::path>(&toPrint)) {
    ss << *p;
  }

  return ss.str();
}

OSArgument::OSArgument() {}

OSArgument::OSArgument(const std::string& name, const OSArgumentType& type, bool required, bool modelDependent)
  : m_name(name), m_displayName(name), m_type(type), m_required(required), m_modelDependent(modelDependent) {}

void OSArgument::onChange() {}

std::map<std::string, OSArgument> convertOSArgumentVectorToMap(const std::vector<OSArgument>& arguments) {
  std::map<std::string, OSArgument> argMap;
  for (const OSArgument& arg : arguments) {
    argMap.insert(std::make_pair(arg.name(), arg.clone()));
  }
  return argMap;
}

}  // namespace openstudio
