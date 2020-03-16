// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the PARAMS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// PARAMS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#include "ParamsExport.h"

#include <map>
#include <string>

class PARAMS_API CParams {
public:
	CParams() = delete;
	CParams(CParams&) = delete;
	CParams(CParams&&) = delete;
	CParams& operator=(CParams&) = delete;
	CParams& operator=(CParams&&) = delete;

	explicit CParams(unsigned long long int id);
	explicit CParams(unsigned long long int id, const std::map<std::string, std::string>& params);

	bool Valid();

	const std::map<std::string, std::string>& GetMap() { return m_params; }
	void Remove(const std::string& key);
	void SetKV(const std::string& key, const std::string& value);

private:
	unsigned long long int m_id = 0;

	std::map<std::string, std::string>	m_params;
};
