#pragma once

#ifdef _WIN32
#ifdef POLYSCOPE_SHARED_LIB
#ifdef POLYSCOPE_EXPORTS
#define POLYSCOPE_API __declspec(dllexport)
#else
#define POLYSCOPE_API __declspec(dllimport)
#endif
#else
#define POLYSCOPE_API
#endif
#else
#define POLYSCOPE_API
#endif