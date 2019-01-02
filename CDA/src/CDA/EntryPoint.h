#pragma once
#include <stdio.h>

#ifdef CDA_PLATFORM_WINDOWS

extern CDA::Application* CDA::CreateApplication();

int main(int argc, char** argv) {

	CDA::Log::Init();
	CDA_CORE_WARN("Initialized Log!");
	int a = 5;
	CDA_INFO("Hello! Var={0}", a);

	auto app = CDA::CreateApplication();
	app->Run();
	delete app;
}

#endif