#include "cdapch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace CDA 
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}