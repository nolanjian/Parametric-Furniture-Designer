// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the MANUFACTURING_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// MANUFACTURING_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MANUFACTURING_EXPORTS
#define MANUFACTURING_API __declspec(dllexport)
#else
#define MANUFACTURING_API __declspec(dllimport)
#endif

// This class is exported from the dll
class MANUFACTURING_API CManufacturing {
public:
	CManufacturing(void);
	// TODO: add your methods here.
};

extern MANUFACTURING_API int nManufacturing;

MANUFACTURING_API int fnManufacturing(void);
