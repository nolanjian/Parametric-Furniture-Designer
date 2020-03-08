// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the PARAMS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// PARAMS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PARAMS_EXPORTS
#define PARAMS_API __declspec(dllexport)
#else
#define PARAMS_API __declspec(dllimport)
#endif

#include <map>
#include <string>

// This class is exported from the dll
class PARAMS_API CParams {
public:
	CParams(void);
	// TODO: add your methods here.
private:
	std::map<std::wstring, std::wstring>	m_params;
};

extern PARAMS_API int nParams;

PARAMS_API int fnParams(void);
