#ifndef openstudio_paths_included
#define openstudio_paths_included

#include <filesystem>

namespace openstudio {

std::filesystem::path getCurrentModulePath();

std::filesystem::path getCurrentModuleDir();

} // namespace openstudio

#endif // openstudio_paths_included
