cmake_minimum_required(VERSION 3.12)

# Configure file to have relative path to the source dir
if (ROOT_INFO)
    set(ROOT_DIR "${CMAKE_SOURCE_DIR}/")
else()
    set(ROOT_DIR "")
endif()

configure_file(includes/ECS/Types.h.in includes/ECS/Types.h)

set(ENGINE jdEngineCore)

set(ENGINE_ECS_SRC
        src/ECS/systems/AssimpModelSystem.cpp
        src/ECS/systems/CameraControlSystem.cpp
        src/ECS/systems/FogSystem.cpp
        src/ECS/systems/LightSystem.cpp
        src/ECS/systems/MaterialSystem.cpp
        src/ECS/systems/ModelSystem2D.cpp
        src/ECS/systems/PlayerControlSystem.cpp
        src/ECS/systems/RenderSystem.cpp
        src/ECS/systems/SkyboxSystem.cpp
        src/ECS/EntityManager.cpp
        src/ECS/EventManager.cpp
        src/ECS/jdEngine.cpp
        src/ECS/SystemManager.cpp
)

set(ENGINE_SRC
        src/Gui/Interface.cpp
        src/Gui/jdGui.cpp
        src/Models/Model.cpp
        src/Models/Terrain.cpp
        src/Render/Primitives.cpp
        src/Render/Sampler.cpp
        src/Render/SamplerManager.cpp
        src/Render/Shader.cpp
        src/Render/ShaderComponent.cpp
        src/Render/ShaderManager.cpp
        src/Render/Texture.cpp
        src/Render/TextureManager.cpp
        src/Render/UniformBuffer.cpp
        src/Render/VertexBuffer.cpp
        src/Scenes/Demo1.cpp
        src/Scenes/SceneManager.cpp
        src/Utils/stb_image.cpp
        src/Utils/Timer.cpp
        src/Display.cpp
        src/GL_ERROR.cpp
)

add_library(${ENGINE} STATIC ${ENGINE_ECS_SRC} ${ENGINE_SRC})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ENGINE_ECS_SRC} ${ENGINE_SRC})

target_include_directories(${ENGINE} PUBLIC includes)
target_include_directories(${ENGINE} PRIVATE src)
target_compile_features(${ENGINE} PUBLIC cxx_std_17)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/assimp ${CMAKE_CURRENT_BINARY_DIR}/assimp)

target_link_libraries(${ENGINE} PUBLIC glfw glad glm assimp)

# Set imgui lib components
set(IMGUI_INCLUDES
        ${CMAKE_SOURCE_DIR}/external/imgui/imgui.h
        ${CMAKE_SOURCE_DIR}/external/imgui/imgui_internal.h
        ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.h
        ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
        ${CMAKE_SOURCE_DIR}/external/imgui/imgui.cpp
        ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp
	    ${CMAKE_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp
	    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_demo.cpp
	    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_tables.cpp
	    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_widgets.cpp
	    ${CMAKE_SOURCE_DIR}/external/imgui/imgui_draw.cpp
)

add_library(ImGui STATIC ${IMGUI_INCLUDES} ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC 
        ${CMAKE_SOURCE_DIR}/external
        ${CMAKE_SOURCE_DIR}/external/imgui
)
target_link_libraries(ImGui PRIVATE glad glfw)

target_link_libraries(${ENGINE} PRIVATE ImGui)