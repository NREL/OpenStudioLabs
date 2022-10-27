#ifndef MEASURE_OSARGUMENT_HPP
#define MEASURE_OSARGUMENT_HPP

#include "MeasureAPI.hpp"

#include <filesystem>
#include <map>
#include <optional>
#include <utility>
#include <variant>
#include <vector>

namespace openstudio {

// clang-format off

  enum class OSArgumentType {
    Boolean = 0,
    Double,
    Quantity,
    Integer,
    String,
    Choice,
    Path,
    Separator
  };

  enum class OSDomainType {
    Interval = 0,
    Enumeration,
  };

// clang-format on

/** Preserves old name for OSArgumentType. \deprecated */
// TODO: JM 2018-11-28: Time to remove no?
using UserScriptArgumentType = OSArgumentType;

/** OSArgument is an argument to an OSMeasure. **/
class MEASURE_API OSArgument
{
 public:
  /** @name Constructors and Destructors */
  //@{

  // ETH@20121211 - Consider adding a constructor or otherwise tweaking the code to fix
  // the issue of having to explicitly call clone (at least in Ruby) in order to setValue.
  // It would be incorrect to always clone on copy since these objects go back and forth
  // to ProjectDatabase.

  OSArgument();
  OSArgument(const std::string& name, const OSArgumentType& type, bool required, bool modelDependent);

  /** Creates a copy with new UUIDs. */
  OSArgument clone() const;

  //@}

  /** @name Factory methods */
  //@{

  /** Creates an OSArgument for bool values. Defaults domainType() to OSDomainType::Enumeration. */
  static OSArgument makeBoolArgument(const std::string& name, bool required = true, bool modelDependent = false);

  /** Creates an OSArgument for double values. Defaults domaintType() to OSDomainType::Interval. */
  static OSArgument makeDoubleArgument(const std::string& name, bool required = true, bool modelDependent = false);

  /** Creates an OSArgument for int values. Defaults domaintType() to OSDomainType::Interval. */
  static OSArgument makeIntegerArgument(const std::string& name, bool required = true, bool modelDependent = false);

  /** Creates an OSArgument for string values. Defaults domainType() to
   *  OSDomainType::Enumeration. */
  static OSArgument makeStringArgument(const std::string& name, bool required = true, bool modelDependent = false);

  /** Creates an OSArgument for path values. Defaults domainType() to OSDomainType::Enumeration. */
  static OSArgument makePathArgument(const std::string& name, bool required = true, bool modelDependent = false);

  /** Creates a separator OSArgument, cannot be used to store a value, cannot be required. */
  static OSArgument makeSeparatorArgument(const std::string& name, bool modelDependent = false);

  //@}

  /** @name Getters */
  //@{

  //openstudio::UUID uuid() const;

  //openstudio::UUID versionUUID() const;

  /** Returns the name of this argument. */
  std::string name() const;

  /** Returns the display name of this argument. */
  std::string displayName() const;

  /** Returns the description of this argument. */
  std::optional<std::string> description() const;

  /** Returns this argument's type. */
  OSArgumentType type() const;

  /** Returns the units of this argument. */
  std::optional<std::string> units() const;

  /** Returns true if this argument is required, that is, if the argument must have a value or
   *  default value for the measure to run properly. */
  bool required() const;

  /** Returns true if this argument's properties can changed based on the specific model used in the
   *  measure.  A choice argument which lists objects in the model would be model dependent. */
  bool modelDependent() const;

  /** Returns true if this argument's value has been set. */
  bool hasValue() const;

  /** Returns this argument's value as a bool. Throws if not hasValue() or if type() !=
   *  OSArgumentType::Boolean. */
  bool valueAsBool() const;

  /** Returns this argument's value as a double. Throws if not hasValue() or if type() !=
   *  OSArgumentType::Double. */
  double valueAsDouble() const;

  /** Returns this argument's value as an int. Throws if not hasValue() or if type() !=
   *  OSArgumentType::Integer. */
  int valueAsInteger() const;

  /** Returns this argument's value as a std::string. If type() != OSArgumentType::String, prints
   *  this argument's value to produce the result. Throws if not hasValue(). */
  std::string valueAsString() const;

  /** Returns this argument's value as an openstudio::path. Throws if not hasValue() or if
   *  type() != OSArgumentType::Path. */
  std::filesystem::path valueAsPath() const;

  //@}
  /** @name Setters */
  //@{

  /** Set the display name of this argument. */
  void setDisplayName(const std::string& displayName);

  /** Set the description of this argument. */
  void setDescription(const std::string& description);

  /** Set the units for this argument. */
  void setUnits(const std::string& units);

  /** Set the value of this argument by passing in data of a particular type. The method will do
   *  nothing and return false if the data is of an incorrect type. These methods do not check
   *  value against the domain (if set), as the domain is just a guideline for users. The string
   *  setter will try to convert the string to the correct type for this argument. Integers can
   *  be used to set the values of arguments of type double. A double  that is really an int (eg: 1.0)
   *  can also be accepted to set integer arguments. */
  bool setValue(bool value);
  /// \overload
  bool setValue(double value);
  /// \overload
  bool setValue(int value);
  /// \overload forwards to std::string& one
  bool setValue(const char* value);
  /// \overload
  bool setValue(const std::string& value);
  /// \overload
  bool setValue(const std::filesystem::path& value);

  void clearValue();

  //@}
  /** @name Serialization */
  //@{

  /** Prints argument data for debugging purposes. */
  std::string print() const;

  /** Returns this argument's value printed to string. If printDefault, returns the default value if
   *  in the case of !hasValue() && hasDefaultValue(). If there is no value to print, the returned
   *  string will be .empty(). */
  std::string printValue() const;

  //@}
 private:
  // ETH@20121211 - Why are these friend declarations here?!
  // JMT@20130124 - Because all of the constructors are private and the SWIG wrapper for pair needs
  // some constructors

  friend class std::map<std::string, OSArgument>;
  friend struct std::pair<std::string, OSArgument>;

#ifdef _LIBCPP_VERSION
  friend struct std::allocator_traits<std::allocator<std::__tree_node<std::__value_type<std::string, OSArgument>, void*>>>;
#endif

#if _MSC_VER >= 1800
  friend struct std::pair<const std::string, OSArgument>;
#elif _MSC_VER >= 1600
  friend struct std::pair<const std::string, OSArgument>;
  friend struct std::_Pair_base<std::string, OSArgument>;
#endif

  // Note JM 2018-11-28:
  // typedef for the std::variant we will use for value, default value, and domain
  // we add std::monostate to allow the variant to be empty basically
  using OSArgumentVariant = std::variant<std::monostate, bool, double, int, std::string, std::filesystem::path>;

  bool setStringInternal(OSArgumentVariant& variant, const std::string& value);

  std::string printOSArgumentVariant(const OSArgumentVariant& toPrint) const;

  // This also OS App related
  void onChange();

  std::string m_name;
  std::string m_displayName;
  std::optional<std::string> m_description;
  OSArgumentType m_type;
  std::optional<std::string> m_units;
  bool m_required;
  bool m_modelDependent;
  OSArgumentVariant m_value;
};

/** \relates OSArgument */
using OptionalOSArgument = std::optional<OSArgument>;

/** \relates OSArgument */
using OSArgumentVector = std::vector<OSArgument>;

/** \relates OSArgument */
using OSArgumentMap = std::map<std::string, OSArgument>;

/** \relates OSArgument */
using UserScriptInfo = std::pair<std::filesystem::path, std::vector<OSArgument>>;

/** Prints argument data to ostream for debugging purposes. \relates OSArgument */
MEASURE_API std::ostream& operator<<(std::ostream& os, const OSArgument& arg);

/** Converts a vector of OSArgument to a map of OSArgument using name as the key. \relates OSArgument */
MEASURE_API std::map<std::string, OSArgument> convertOSArgumentVectorToMap(const std::vector<OSArgument>& arguments);

}  // namespace openstudio

#endif  // MEASURE_OSARGUMENT_HPP
