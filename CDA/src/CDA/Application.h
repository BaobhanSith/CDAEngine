#pragma once

#include "Core.h"

#include "Window.h"
#include "CDA/LayerStack.h"
#include "Events/Event.h"
#include "CDA/Events/ApplicationEvent.h"

namespace CDA {

	class CDA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in client
	Application* CreateApplication();
}

