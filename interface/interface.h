#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../objects/Camera.h"
#include "../render/SkyBox.h"
#include "../render/Renderer.h"

extern float deltaTime;

class RenderGui
{
public:
	static void ImGuiInit(GLFWwindow* window, const Renderer& rendererArg);
	static void ImGuiDrawWindows(Camera& camera, glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float& shininess, Skybox& skybox);

private:
	// See and change camera position, speed etc.
	static void CameraMenu(Camera& camera);
	// Performance info : framerate etc.
	static void PerformanceMenu();
	// Shows the various controls
	static void KeysMenu();
	// Enables modifications of some shader values
	static void ShaderSettings(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular, float& shininess);
	// Displays the scene's lights and allows real-time modification of their attributes
	static void LightSettings();
	// Display menu for changing skyboxes
	static void SkyboxSettings(Skybox& skybox);

	//@Data
	// NVIDIA GPU only, serves to query the amount of total and available VRAM
	inline static GLint total_mem_kb = 0;
	inline static GLint cur_avail_mem_kb = 0;

	// For calculating the FPS, the time taken to render the last 100 frames is used
	inline static float lastFrames[100];
	inline static int frameIndex = 0;
	inline static float timeToFrame = 1;

	// Used for dynamically showing existing lights and enabling their realtime modification
	inline static int sLightID = 0;
	inline static std::vector<DirectionalLight*> dirLights;
	inline static std::vector<PointLight*> pointLights;
	inline static std::vector<SpotLight*> spotLights;

	// An array containing the choice names for the different skyboxes
	inline static const char* skyboxes[] = { "Sky", "Grass", "Night" };
	inline static int skbox_current_idx = 0; // Selection data index
};