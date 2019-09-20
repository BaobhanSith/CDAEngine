#include <CDA.h>

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public CDA::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		m_VertexArray.reset(CDA::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		CDA::Ref<CDA::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(CDA::VertexBuffer::Create(vertices, sizeof(vertices)));

		CDA::BufferLayout layout = {
			{ CDA::ShaderDataType::Float3, "a_Position" },
			{ CDA::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		CDA::Ref<CDA::IndexBuffer> indexBuffer;
		indexBuffer.reset(CDA::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(CDA::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		CDA::Ref<CDA::VertexBuffer> squareVB;
		squareVB.reset(CDA::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ CDA::ShaderDataType::Float3, "a_Position" },
			{ CDA::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		CDA::Ref<CDA::IndexBuffer> squareIB;
		squareIB.reset(CDA::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
	
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
				color = vec4(u_Color * v_Position + 0.5, 1.0);
			}
		)";

		m_Shader = CDA::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(v_Position * 0.5, 1.0);
				color = vec4(u_Color, 1.0f);
			}
		)";

		m_flatColorShader = CDA::Shader::Create("FlatColor", flatColorVertexSrc, flatColorShaderFragmentSrc);

		auto TextureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		m_Texture = CDA::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = CDA::Texture2D::Create("assets/textures/Logo.png");

		std::dynamic_pointer_cast<CDA::OpenGLShader>(TextureShader)->Bind();
		std::dynamic_pointer_cast<CDA::OpenGLShader>(TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(CDA::Timestep ts) override
	{	
		m_CameraController.OnUpdate(ts);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		
		CDA::RenderCommand::SetClearColor(glm::vec4(m_BackgroundColor, 1.0f));
		CDA::RenderCommand::Clear();

		CDA::Renderer::BeginScene(m_CameraController.GetCamera());

		std::dynamic_pointer_cast<CDA::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<CDA::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_TriangleColor);

		std::dynamic_pointer_cast<CDA::OpenGLShader>(m_flatColorShader)->Bind();
		std::dynamic_pointer_cast<CDA::OpenGLShader>(m_flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				CDA::Renderer::Submit(m_flatColorShader, m_SquareVA, transform);
			}
		}

		//CDA::Renderer::Submit(m_Shader, m_VertexArray);

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		CDA::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_ChernoLogoTexture->Bind();
		CDA::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		CDA::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_TriangleColor));
		ImGui::ColorEdit3("Background Color", glm::value_ptr(m_BackgroundColor));
		ImGui::End();
	}

	void OnEvent(CDA::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	CDA::ShaderLibrary m_ShaderLibrary;
	CDA::Ref<CDA::Shader> m_Shader;
	CDA::Ref<CDA::VertexArray> m_VertexArray;

	CDA::Ref<CDA::VertexArray> m_SquareVA;
	CDA::Ref<CDA::Shader> m_flatColorShader;

	CDA::Ref<CDA::Texture2D> m_Texture, m_ChernoLogoTexture;

	CDA::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
	glm::vec3 m_TriangleColor = { 0.4f, 0.6f, 0.4f };
	glm::vec3 m_BackgroundColor = { 0.1f, 0.1f, 0.1f };
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