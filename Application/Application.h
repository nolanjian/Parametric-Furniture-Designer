/*****************************************************************//**
 * \file   Application.h
 * \brief  
 * 
 * \author NolanJian
 * \e-mail NolanJian@163.com
 * \date   2020/07/13
 * 
 *********************************************************************/
#pragma once
#include <Application/Command.h>

#include <QtWidgets/QMainWindow>
#include "ui_Application.h"

namespace PFD
{
    namespace GUI
    {
		class Application : public QMainWindow
		{
			Q_OBJECT

		public:
			Application(QWidget* parent = Q_NULLPTR);

			bool Initialize();
			bool CreateWidgets();

			void CreateActions();

		public slots:
			void ActionTriggered();
			void testSlots();

		protected:
			void HandleCommand(CommandID id);

		private:
			Ui::ApplicationClass ui;

			std::map<void*, CommandID>	m_mapActions;
		};
    }
}