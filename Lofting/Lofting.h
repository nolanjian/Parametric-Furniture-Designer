// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the LOFTING_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// LOFTING_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LOFTING_EXPORTS
#define LOFTING_API __declspec(dllexport)
#else
#define LOFTING_API __declspec(dllimport)
#endif

// This class is exported from the dll
class LOFTING_API CLofting {
public:
	CLofting(void);
	// TODO: add your methods here.
};

extern LOFTING_API int nLofting;

LOFTING_API int fnLofting(void);
