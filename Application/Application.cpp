#include "Application.h"
#include <Scene/OpenGLWidget.h>

namespace PFD
{
	namespace GUI
	{
		Application::Application(QWidget* parent)
			: QMainWindow(parent)
		{
			ui.setupUi(this);
		}

		void Application::testSlots()
		{
			int nn = 0;
			nn = 99;
		}

		void Application::HandleCommand(CommandID id)
		{
			switch (id)
			{
			case PFD::GUI::PFD_CMD_FILE_NEW:
				break;
			case PFD::GUI::PFD_CMD_FILE_OPEN:
				break;
			case PFD::GUI::PFD_CMD_FILE_SAVE:
				break;
			case PFD::GUI::PFD_CMD_FILE_SAVEAS:
				break;
			case PFD::GUI::PFD_CMD_END:
				break;
			default:
				assert(false);
				break;
			}
		}

		bool Application::Initialize()
		{
			bool bRet = CreateWidgets();

			return true;
		}

		bool Application::CreateWidgets()
		{
			QMenu* menuFile = menuBar()->addMenu("&File");
			QAction* action = menuFile->addAction("&Open");

			PFD::Scene::OpenGLWidget* oglWidget = new PFD::Scene::OpenGLWidget();
			setCentralWidget(oglWidget);

			connect(action, SIGNAL(triggered()), oglWidget, SLOT(OpenFile()));

			QMenu* menuView = menuBar()->addMenu("&View");
			QMenu* menuProjection = menuView->addMenu("Projection");
			QAction* action = menuProjection->addAction("&Open");
			QAction* action = menuProjection->addAction("&Open");

			return true;
		}

		void Application::CreateActions()
		{
			assert(PFD_CMD_BEGIN == 0);

			for (size_t ii = 0; ii < PFD_CMD_END; ++ii)
			{
				QAction* pAction = new QAction();
				addAction(pAction);
				connect(pAction, SIGNAL(triggered()), this, SLOT(ActionTriggered()));
				
				m_mapActions[pAction] = (CommandID)ii;
			}
		}

		void Application::ActionTriggered()
		{
			QObject* pAction = sender();
			if (pAction)
			{
				CommandID id = m_mapActions[pAction];
				assert(id >= PFD_CMD_BEGIN && id <= PFD_CMD_END);


			}
		}
	}
}