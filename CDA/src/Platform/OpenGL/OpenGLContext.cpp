#include "cdapch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gl/GL.h>

namespace CDA {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CDA_CORE_ASSERTS(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CDA_CORE_ASSERTS(status, "Failed to initialize Glad!");

		CDA_CORE_INFO("OpenGL Info: ");
		CDA_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		CDA_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		CDA_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}