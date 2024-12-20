#ifndef openstudio_paths_included
#define openstudio_paths_included

#include <filesystem>

namespace fs = std::filesystem;

namespace openstudio {

using path = std::filesystem::path;

openstudio::path getCurrentModulePath();

openstudio::path getCurrentModuleDir();

}  // namespace openstudio

#endif  // openstudio_paths_included
