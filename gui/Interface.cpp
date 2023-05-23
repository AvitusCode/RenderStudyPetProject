#include "Interface.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../ECS/jdEngine.h"
#include "../Events/Body2DCreationEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/WindowEvent.h"
#include "../Display.h"

#include "../render/Primitives.h"

namespace jd
{

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

	void Interface::StatusMenu()
	{
		if (!jd::Engine::IsDebugMode()) {
			return;
		}

		GLint total_mem_kb{};
		GLint cur_avail_mem_kb{};
		glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &total_mem_kb);
		glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &cur_avail_mem_kb);

		ImGui::Begin("Status");

		ImGui::Text("Total VRAM (MB): %i", total_mem_kb / 1000);
		ImGui::Text("Available VRAM (MB): %i", cur_avail_mem_kb / 1000);
		ImGui::Text("Used VRAM (MB): %i", (total_mem_kb - cur_avail_mem_kb) / 1000);

		ImGui::End();
	}

	void Interface::ProgMenu()
	{
		if (!jd::Engine::IsDebugMode()) {
			return;
		}

		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton;
		static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		static bool status_bar = false;

		if (ImGui::BeginMenuBar())
		{

			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Add model file", nullptr)) 
				{

				}
				if (ImGui::MenuItem("Set existing model", nullptr)) 
				{

				}

				ImGui::Separator();
				if (ImGui::MenuItem("Exit", nullptr)) 
				{
					auto& engine = jd::Engine::getEngine();
					WindowClosedEvent eventClose{};
					engine.SendEvent(&eventClose);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scene"))
			{
				if (ImGui::MenuItem("Next Scene...", nullptr))
				{

				}
				if (ImGui::MenuItem("Prev Scene...", nullptr))
				{

				}
				if (ImGui::MenuItem("Open/close statusBar", nullptr))
				{
					status_bar = !status_bar;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		ImGui::End();

		if (status_bar) {
			StatusMenu();
		}
	}
}