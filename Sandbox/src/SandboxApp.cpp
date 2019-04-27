#include <CDA.h>

class ExampleLayer : public CDA::Layer {
public:
	ExampleLayer()
		: Layer("Example") {

	}

	void OnUpdate() override {
		CDA_INFO("ExampleLayer::Update");
	}

	void OnEvent(CDA::Event& event) override {
		//CDA_TRACE("{0}", event);
	}
};

class Sandbox : public CDA::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new CDA::ImGuiLayer());
	}

	~Sandbox() {

	}
};

CDA::Application* CDA::CreateApplication() {
	return new Sandbox();
}