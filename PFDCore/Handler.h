#pragma once
#include "Model.h"
#include "PFDCoreExport.h"

namespace PFDCore
{

	class Handler : public Model
	{
	public:
		Handler();
		virtual ~Handler();
		//virtual bool InitFromParams() override;
	};
}