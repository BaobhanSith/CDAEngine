#pragma once
#include "Core.h"

namespace CDA {

	class CDA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in client
	Application* CreateApplication();
}

