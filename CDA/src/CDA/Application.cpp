#include "cdapch.h"
#include "Application.h"

#include "CDA/Events/ApplicationEvent.h"
#include "CDA/Log.h"

namespace CDA {

	Application::Application()
	{
	}


	Application::~Application()
	{
	}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		CDA_TRACE(e);
		while (true);
	}
}