#include "cdapch.h"

#include "Shader.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace CDA {

	Shader* Shader::Create(const std::string& filepath) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CDA_CORE_ASSERTS(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLShader(filepath);
		}

		CDA_CORE_ASSERTS(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CDA_CORE_ASSERTS(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		CDA_CORE_ASSERTS(false, "Unknown RendererAPI!");
		return nullptr;
	}
}