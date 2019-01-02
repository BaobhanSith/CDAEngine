#include <CDA.h>

class Sandbox : public CDA::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

CDA::Application* CDA::CreateApplication() {
	return new Sandbox();
}