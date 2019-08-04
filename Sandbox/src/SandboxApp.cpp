#include <CDA.h>

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public CDA::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(CDA::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<CDA::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(CDA::VertexBuffer::Create(vertices, sizeof(vertices)));

		CDA::BufferLayout layout = {
			{ CDA::ShaderDataType::Float3, "a_Position" },
			{ CDA::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<CDA::IndexBuffer> indexBuffer;
		indexBuffer.reset(CDA::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(CDA::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<CDA::VertexBuffer> squareVB;
		squareVB.reset(CDA::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ CDA::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<CDA::IndexBuffer> squareIB;
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

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(CDA::Shader::Create(vertexSrc, fragmentSrc));

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
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = vec4(u_Color, 0.0f);
			}
		)";

		m_flatColorShader.reset(CDA::Shader::Create(flatColorVertexSrc, flatColorShaderFragmentSrc));
	}

	void OnUpdate(CDA::Timestep ts) override
	{	
		float time = ts;

		if (CDA::Input::IsKeyPressed(CDA_KEY_LEFT)) 
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (CDA::Input::IsKeyPressed(CDA_KEY_RIGHT)) 
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		
		if (CDA::Input::IsKeyPressed(CDA_KEY_UP)) 
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (CDA::Input::IsKeyPressed(CDA_KEY_DOWN)) 
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (CDA::Input::IsKeyPressed(CDA_KEY_COMMA))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (CDA::Input::IsKeyPressed(CDA_KEY_PERIOD))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		

		CDA::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		CDA::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		CDA::Renderer::BeginScene(m_Camera);

		std::dynamic_pointer_cast<CDA::OpenGLShader>(m_flatColorShader)->Bind();
		std::dynamic_pointer_cast<CDA::OpenGLShader>(m_flatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				CDA::Renderer::Submit(m_flatColorShader, m_SquareVA, transform);
			}
		}

		CDA::Renderer::Submit(m_Shader, m_VertexArray);

		CDA::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(CDA::Event& event) override
	{
	}

private:
	std::shared_ptr<CDA::Shader> m_Shader;
	std::shared_ptr<CDA::VertexArray> m_VertexArray;

	std::shared_ptr<CDA::VertexArray> m_SquareVA;
	std::shared_ptr<CDA::Shader> m_flatColorShader;

	CDA::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
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