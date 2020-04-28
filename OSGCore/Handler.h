#pragma once
#include "Model.h"
#include "OSGCoreExport.h"

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