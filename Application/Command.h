/*****************************************************************//**
 * \file   Command.h
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/07/13
 * 
 *********************************************************************/
#pragma once

namespace PFD
{
	namespace GUI
	{
		enum CommandID
		{
			PFD_CMD_BEGIN = 0,
			PFD_CMD_FILE_NEW = PFD_CMD_BEGIN,
			PFD_CMD_FILE_OPEN,
			PFD_CMD_FILE_SAVE,
			PFD_CMD_FILE_SAVEAS,

			PFD_CMD_END,
			PFD_CMD_COUNT = PFD_CMD_END
		};
	}
}