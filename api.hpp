#ifndef UTILITIES_UTILITIESAPI_HPP
#define UTILITIES_UTILITIESAPI_HPP

#if (_WIN32 || _MSC_VER)

#if defined(DLLDEMO_EXPORTS)
#define DLLDEMO_API __declspec(dllexport)
#define DLLDEMO_TEMPLATE_EXT
#else
#define DLLDEMO_API __declspec(dllimport)
#define DLLDEMO_TEMPLATE_EXT extern
#endif

#else

#define DLLDEMO_API
#if defined(DLLDEMO_EXPORTS)
#define DLLDEMO_TEMPLATE_EXT
#else
#define DLLDEMO_TEMPLATE_EXT extern
#endif

#endif

// #else
//
// #define DLLDEMO_API
// #define DLLDEMO_TEMPLATE_EXT

#endif
