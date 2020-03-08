// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the MUPARSERX_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// MUPARSERX_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MUPARSERX_EXPORTS
#define MUPARSERX_API __declspec(dllexport)
#else
#define MUPARSERX_API __declspec(dllimport)
#endif

// This class is exported from the dll
class MUPARSERX_API Cmuparserx {
public:
	Cmuparserx(void);
	// TODO: add your methods here.
};

extern MUPARSERX_API int nmuparserx;

MUPARSERX_API int fnmuparserx(void);
