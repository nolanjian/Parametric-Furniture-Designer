#pragma once

#include "WXIncluding.h"
#include "Utils.h"

namespace PFD
{
	namespace GUI
	{
		class App : public wxApp
		{
		public:
			App();
			virtual ~App();
			virtual bool OnInit() override;
			virtual int OnExit() override;
		protected:
			std::shared_ptr<spdlog::logger> logger = spdlog::get(PFD_LOGGER);
		};
	}
}