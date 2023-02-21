#include <iostream>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../main.h"
#include "interface.h"
#include "../light/LightManager.h"
#include "../light/DirLight.h"
#include "../light/PointLight.h"
#include "../light/SpotLight.h"

#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

void RenderGui::ImGuiInit(GLFWwindow* window, const Renderer& rendererArg)
{
	// Setup Dear ImgGui context
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup platform/renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	// Setup Dear ImgUi style
	ImGui::StyleColorsDark();

	sLightID = rendererArg.getLightManager().getShaderId();
	dirLights = rendererArg.getLightManager().getDirLights();
	pointLights = rendererArg.getLightManager().getPointLights();
	spotLights = rendererArg.getLightManager().getSpotLights();
}

void RenderGui::ImGuiDrawWindows(Camera& camera, glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float& shininess, Skybox& skybox)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	CameraMenu(camera);
	PerformanceMenu();
	KeysMenu();
	ShaderSettings(ambient, diffuse, specular, shininess);
	LightSettings();
	SkyboxSettings(skybox);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void RenderGui::CameraMenu(Camera& camera)
{
	ImGui::Begin("Camera");

	// Camera FOV value
	ImGui::PushItemWidth(100.0f);
	float temp = camera.getZoom();
	ImGui::SliderFloat("FOV", &temp, 1.0f, 90.0f);
	camera.setZoom(temp);

	ImGui::PushItemWidth(150.0f);
	glm::vec3 position = camera.getPosition();
	ImGui::DragFloat3("Position", &position[0]);
	camera.getPosition() = position;

	ImGui::PushItemWidth(100.0f);
	temp = camera.getMSpeed();
	ImGui::InputFloat("Speed", &temp);
	camera.setMSpeed(temp);
	temp = camera.getMSentivity();
	ImGui::InputFloat("Sensitivity", &temp);
	camera.setMSentivity(temp);

	ImGui::NewLine();
	if (ImGui::Button("Reset"))
	{
		camera.setZoom(Camera::ZOOM);
		camera.getPosition() = glm::vec3(0.0f, 0.0f, 0.0f);
		camera.setMSpeed(Camera::SPEED);
		camera.setMSentivity(Camera::SENSITIVITY);
	}

	ImGui::End();
}

void RenderGui::PerformanceMenu()
{
	glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &total_mem_kb);
	glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &cur_avail_mem_kb);

	ImGui::Begin("Performance");
	ImGui::Text("Frame render time: %.2f ms", timeToFrame * 1000);

	lastFrames[frameIndex++] = deltaTime;

	if (frameIndex == 100)
	{
		frameIndex = 0;
		float frameTimeSum = 0.0f;

		for (size_t i = 0; i < 100; i++)
		{
			frameTimeSum += lastFrames[i];
		}
		timeToFrame = frameTimeSum / 100;
	}

	ImGui::Text("Framerate: %i", (int)(1 / timeToFrame));
	ImGui::Text("Total VRAM (MB): %i", total_mem_kb / 1000);
	ImGui::Text("Available VRAM (MB): %i", cur_avail_mem_kb / 1000);
	ImGui::Text("Used VRAM (MB): %i", (total_mem_kb - cur_avail_mem_kb) / 1000);

	ImGui::End();
}

void RenderGui::KeysMenu()
{
	ImGui::Begin("Controls");

	ImGui::Text("Look around : Mouse");
	ImGui::Text("Movement : WASD");
	ImGui::Text("Toggle cursor : Q");
	ImGui::Text("Next scene: Right button");
	ImGui::Text("Change FOV : Scroll wheel");
	ImGui::Text("Toggle wireframe : F10");
	ImGui::Text("Toggle fullscreen : F11");
	ImGui::Text("Quit : Esc");

	ImGui::End();
}

void RenderGui::ShaderSettings(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float& shininess)
{
	ImGui::Begin("Shader settings");

	ImGui::ColorEdit3("Ambient", &ambient[0]);
	ImGui::ColorEdit3("Diffuse", &diffuse[0]);
	ImGui::ColorEdit3("Specular", &specular[0]);

	ImGui::DragFloat("Shininess", &shininess, 1.0f, 1.0f, 512.0f);

	ImGui::End();
}

void RenderGui::LightSettings()
{
	ImGui::Begin("Light settings");

	for (unsigned int i = 0; i < dirLights.size(); i++)
	{
		if (ImGui::CollapsingHeader(("DirectionalLight " + std::to_string(i)).c_str()))
		{
			bool modified = false;

			if (ImGui::ColorEdit3(("DL_Amb " + std::to_string(i)).c_str(), &(dirLights[i]->ambientColor[0]))) {
				modified = true;
			}
			if (ImGui::ColorEdit3(("DL_Diff " + std::to_string(i)).c_str(), &(dirLights[i]->diffuseColor[0]))) {
				modified = true;
			}
			if (ImGui::ColorEdit3(("DL_Spec " + std::to_string(i)).c_str(), &(dirLights[i]->specularColor[0]))) {
				modified = true;
			}

			if (ImGui::DragFloat3(("DL_Dir " + std::to_string(i)).c_str(), &(dirLights[i]->direction[0]), 0.002f)) {
				modified = true;
			}

			if (modified){
				dirLights[i]->sendToShader(sLightID, i);
			}
		}
	}

	for (unsigned int i = 0; i < pointLights.size(); i++)
	{
		if (ImGui::CollapsingHeader(("PointLight " + std::to_string(i)).c_str()))
		{
			bool modified = false;

			if (ImGui::ColorEdit3(("PL_Amb " + std::to_string(i)).c_str(), &(pointLights[i]->ambientColor[0]))) {
				modified = true;
			}
			if (ImGui::ColorEdit3(("PL_Diff " + std::to_string(i)).c_str(), &(pointLights[i]->diffuseColor[0]))) {
				modified = true;
			}
			if (ImGui::ColorEdit3(("PL_Spec " + std::to_string(i)).c_str(), &(pointLights[i]->specularColor[0]))) {
				modified = true;
			}

			if (ImGui::DragFloat3(("PL_Pos " + std::to_string(i)).c_str(), &(pointLights[i]->position[0]))) {
				modified = true;
			}

			if (ImGui::DragFloat(("PL_Const " + std::to_string(i)).c_str(), &(pointLights[i]->constant), 0.002f, 0.0f, 1.0f)) {
				modified = true;
			}
			if (ImGui::DragFloat(("PL_Lin " + std::to_string(i)).c_str(), &(pointLights[i]->linear), 0.002f, 0.0f, 1.0f)) {
				modified = true;
			}
			if (ImGui::DragFloat(("PL_Quad " + std::to_string(i)).c_str(), &(pointLights[i]->quadratic), 0.002f, 0.001f, 1.0f)) {
				modified = true;
			}

			if (modified){
				pointLights[i]->sendToShader(sLightID, i);
			}
		}
	}

	for (unsigned int i = 0; i < spotLights.size(); i++)
	{
		if (ImGui::CollapsingHeader(("SpotLight " + std::to_string(i)).c_str()))
		{
			bool modified = false;

			if (ImGui::ColorEdit3(("SL_Amb " + std::to_string(i)).c_str(), &(spotLights[i]->ambientColor[0]))) {
				modified = true;
			}
			if (ImGui::ColorEdit3(("SL_Diff " + std::to_string(i)).c_str(), &(spotLights[i]->diffuseColor[0]))) {
				modified = true;
			}
			if (ImGui::ColorEdit3(("SL_Spec " + std::to_string(i)).c_str(), &(spotLights[i]->specularColor[0]))) {
				modified = true;
			}

			if (ImGui::DragFloat3(("SL_Pos " + std::to_string(i)).c_str(), &(spotLights[i]->position[0]))) {
				modified = true;
			}
			if (ImGui::DragFloat3(("SL_Dir " + std::to_string(i)).c_str(), &(spotLights[i]->direction[0]), 0.002f)) {
				modified = true;
			}

			if (ImGui::DragFloat(("SL_Const " + std::to_string(i)).c_str(), &(spotLights[i]->constant), 0.002f, 0.0f, 1.0f)) {
				modified = true;
			}
			if (ImGui::DragFloat(("SL_Lin " + std::to_string(i)).c_str(), &(spotLights[i]->linear), 0.002f, 0.0f, 1.0f)) {
				modified = true;
			}
			if (ImGui::DragFloat(("SL_Quad " + std::to_string(i)).c_str(), &(spotLights[i]->quadratic), 0.002f, 0.001f, 1.0f)) {
				modified = true;
			}

			if (ImGui::DragFloat(("SL_cutOff " + std::to_string(i)).c_str(), &(spotLights[i]->cutOff), 0.002f, 0.001f, 1.0f)) {
				modified = true;
			}
			if (ImGui::DragFloat(("SL_outerCutOff " + std::to_string(i)).c_str(), &(spotLights[i]->outerCutOff), 0.002f, 0.001f, 1.0f)) {
				modified = true;
			}

			if (modified){
				spotLights[i]->sendToShader(sLightID, i);
			}
		}
	}

	ImGui::End();
}

// Does not work
void RenderGui::SkyboxSettings(Skybox& skybox)
{
	ImGui::Begin("Skybox settings");

	const char* prev_cal = skyboxes[skbox_current_idx];
	if (ImGui::BeginCombo("Skybox", prev_cal))
	{
		for (uint32_t i = 0; i < IM_ARRAYSIZE(skyboxes); i++)
		{
			const bool is_selected = (skbox_current_idx == i);
			if (ImGui::Selectable(skyboxes[i], is_selected))
			{
				skbox_current_idx = i;
				Cubemap cubemap;
				switch (skbox_current_idx)
				{
				case 0:
					cubemap = Cubemap("res/img/skybox/sky/");
					break;
				case 1:
					cubemap = Cubemap("res/img/skybox/grass/");
					break;
				case 2:
					cubemap = Cubemap("res/img/skybox/night/");
					break;
				default:
					std::cerr << "ERROR: skybox changing" << std::endl;
					break;
				}

				cubemap.setupObject();
				skybox.getCubemap() = std::move(cubemap);
			}

			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	ImGui::End();
}