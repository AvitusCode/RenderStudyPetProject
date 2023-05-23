#pragma once
#include <glm/glm.hpp>

namespace jd
{
	class imGui final
	{
	public:
		static imGui& instance() {
			static imGui m_gui{};
			return m_gui;
		}
		imGui(const imGui&) = delete;
		imGui& operator=(const imGui&) = delete;
		~imGui();

		void OnInit();
		void OnDispouse();

		void GuiBegin();
		void GuiEnd();

	private:
		imGui();
		void SetDarkThemeColors();
		bool status = false;
	};
}