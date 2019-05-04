#pragma once

#include "CDA/Layer.h"

#include "CDA/Events/KeyEvent.h"
#include "CDA/Events/ApplicationEvent.h"
#include "CDA/Events/MouseEvent.h"

namespace CDA {
	class CDA_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}