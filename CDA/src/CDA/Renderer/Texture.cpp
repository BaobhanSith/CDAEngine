#include "cdapch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace CDA {
	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    CDA_CORE_ASSERTS(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLTexture2D>(path);
		}

		CDA_CORE_ASSERTS(false, "Unknown RendererAPI!");
		return nullptr;
	}
}