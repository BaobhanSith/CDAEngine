#include <CDA.h>

#include "imgui/imgui.h"

class ExampleLayer : public CDA::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (CDA::Input::IsKeyPressed(CDA_KEY_TAB))
			CDA_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override
	{
		//ImGui::Begin("Test");
		//ImGui::Text("Hello World");
		//ImGui::End();
	}

	void OnEvent(CDA::Event& event) override
	{
		if (event.GetEventType() == CDA::EventType::KeyPressed)
		{
			CDA::KeyPressedEvent& e = (CDA::KeyPressedEvent&)event;
			if (e.GetKeyCode() == CDA_KEY_TAB)
				CDA_TRACE("Tab key is pressed (event)!");
			CDA_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

};

class Sandbox : public CDA::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

CDA::Application* CDA::CreateApplication()
{
	return new Sandbox();
}