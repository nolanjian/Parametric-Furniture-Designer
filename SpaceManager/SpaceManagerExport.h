#pragma once

#ifdef PFD_SPACEMANAGER_EXPORTS
#define SPACEMANAGER_API __declspec(dllexport)
#else
#define SPACEMANAGER_API __declspec(dllimport)
#endif