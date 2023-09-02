#include "Gui/Interface.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "Gui/imgui_notify.h"

#include "ECS/jdEngine.h"
#include "Events/EntityCreationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/WindowEvent.h"
#include "Display.h"
#include "Utils/File.h"
#include "Utils/logger.h"
#include "Utils/ThreadPool.h"

#include "Render/Primitives.h"
#include "ECS/components/Renderable.h"
#include "ECS/components/AssimpModel.h"
#include "ECS/components/Transform.h"
#include "ECS/components/Material.h"
#include "ECS/components/Camera.h"


void loadModelInterface(std::string modelPath, unsigned int flag);
bool AddModelMenu(sm::model::state::Selectable& state, unsigned int& flag);
bool SetModelMenu();

namespace sm::model
{
	MaybeState onEvent(state::Idle& state, const event::Add& evt) {
		LOG(INFO) << jd::ModelMenu::info() << " Idle -> Selectable";
		return state::Selectable{};
	}

	MaybeState onEvent(state::Idle& state, const event::Choose& evt) {
		auto& engine = jd::Engine::getEngine();
		auto& modelManager = engine.getModelManager();

		if (modelManager.empty()) {
			return std::nullopt;
		}

		LOG(INFO) << jd::ModelMenu::info() << " Idle -> Work";
		return state::Work{};
	}

	MaybeState onEvent(state::Selectable& state, const event::Draw& evt) {
		ImGui::Begin("AddModel");

		if (unsigned int flag = aiProcess_Triangulate | aiProcess_FlipUVs; AddModelMenu(state, flag))
		{
			auto model_path = jd::filesystem::FileOpen();
			if (!model_path.has_value()) {
				LOG(WARNING) << "cannot loading model! problem with file path";
			}
			else
			{
				loadModelInterface(*model_path, flag);
			}

			LOG(INFO) << jd::ModelMenu::info() << " Selectable -> Idle";
			ImGui::End();
			return state::Idle{};
		}

		if (ImGui::Button("Close")) {
			ImGui::End();
			LOG(INFO) << jd::ModelMenu::info() << " Selectable -> Idle";
			return state::Idle{};
		}

		ImGui::End();
		return std::nullopt;
	}

	MaybeState onEvent(state::Work& state, const event::Draw& evt) {
		if (SetModelMenu()) {
			LOG(INFO) << jd::ModelMenu::info() << " Work -> Idle";
			return state::Idle{};
		}
		return std::nullopt;
	}

	template<typename Ts, typename Te>
	MaybeState onEvent(Ts& state, const Te& evt) {
		return std::nullopt;
	}
}

namespace jd
{
	void ModelMenu::processEvent(const sm::model::Event& event)
	{
		using namespace sm::model;

		auto nextState = std::visit(overload{
			[](auto& state, const auto& evt) {
				    return onEvent(state, evt);
				}
			}, state_, event);

		if (nextState) {
			state_ = *nextState;
		}
	}

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
					modelMenu.processEvent(sm::model::event::Add {});
				}
				if (ImGui::MenuItem("Set existing model", nullptr)) 
				{
					modelMenu.processEvent(sm::model::event::Choose {});
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

		modelMenu.processEvent(sm::model::event::Draw {});

        // It in neccessary to be placed in the end of the render code!
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 50.f); // Round borders
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.0f / 255.0f, 43.0f / 255.0f, 43.0f / 255.0f, 100.0f / 255.0f)); // Background color
		ImGui::RenderNotifications(); // <-- Here we render all notifications
		ImGui::PopStyleVar(1); 
		ImGui::PopStyleColor(1);
	}
}

void loadModelInterface(std::string modelPath, unsigned int flag)
{
	auto& engine = jd::Engine::getEngine();
	auto& modelManager = engine.getModelManager();
	std::string modelName = modelPath.substr(modelPath.find_last_of("/") + 1, modelPath.find_last_of("."));
	// If model already cached then return from the func
	if (!modelManager.cachedModelName(modelName)) {
		return;
	}

	auto& tp = jd::utils::ThreadPool::Instance();
	tp.postToThread([modelPath = std::move(modelPath), modelName = std::move(modelName), flag] {
		auto& engine = jd::Engine::getEngine();
	    auto& modelManager = engine.getModelManager();
		
		try {
			modelManager.loadModelObj(modelPath, modelName, flag);
	    }
	    catch (std::exception& ex) {
		    LOG(ERROR) << ex.what();
			ImGui::InsertNotification({ ImGuiToastType_Error, 10000, "Model with name %s was NOT loaded! error = %s", modelName.c_str(), ex.what() });
			modelManager.deleteModel(modelName);
			return;
	    }

	    LOG(INFO) << "successfully load model: " << modelName;
		ImGui::InsertNotification({ ImGuiToastType_Success, 10000, "Model with name %s was successfully loaded!", modelName.c_str()});
	});
}

bool AddModelMenu(sm::model::state::Selectable& state, unsigned int& flag)
{
	ImGui::Checkbox("CalcTangentSpace: ", &state.calcTangentSpace);
	ImGui::Checkbox("GenSmoothNormals: ", &state.getSmoothNormal);

	bool resultFlag = false;
	if (ImGui::Button("Confirm"))
	{
		resultFlag = true;
		if (state.calcTangentSpace) {
			flag |= aiProcess_CalcTangentSpace;
		}
		if (state.getSmoothNormal) {
			flag |= aiProcess_GenSmoothNormals;
		}
	}

	return resultFlag;
}

bool SetModelMenu()
{
	auto& engine = jd::Engine::getEngine();
	auto& modelManager = engine.getModelManager();
	const auto& models_menu = modelManager.getModelsMenu();
	static uint32_t model_current_idx = 0;

	if (models_menu.empty()) {
		return false;
	}

	ImGui::Begin("SetModelMenu");

	const std::string& prev_cal = models_menu[model_current_idx];
	if (ImGui::BeginCombo("Models", prev_cal.c_str()))
	{
		for (uint32_t i = 0; i < models_menu.size(); i++)
		{
			const bool is_selected = (model_current_idx == i);
			if (ImGui::Selectable(models_menu[i].c_str(), is_selected))
			{
				model_current_idx = i;
			}

			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	if (ImGui::Button("Confirm"))
	{
		JdModel amodel{ modelManager.getModel(models_menu[model_current_idx]) };
		Renderable renderable;
		renderable.shader = std::addressof(engine.getShaderManager().getShaderProgram("shader_default"));
		// Get info from the camera

		auto entity = engine.CreateEntity();
		engine.AddComponent<Renderable>(entity, std::move(renderable));
		engine.AddComponent<JdModel>(entity, std::move(amodel));
		engine.AddComponent<Transform>(entity,
			Transform{ glm::vec3(0.0f, 0.0f, 0.0f),
					   glm::vec3(0.0f, 0.0f, 0.0f),
					   glm::vec3(0.5f, 0.5f, 0.5f) });
		engine.AddComponent<Material>(entity, Material{ glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32.0f }); // Standart material

		EntityTransformEvent event{ entity };
		engine.SendEvent(&event);
	}

	if (ImGui::Button("Close"))
	{
		ImGui::End();
		return true;
	}

	ImGui::End();
	return false;
}