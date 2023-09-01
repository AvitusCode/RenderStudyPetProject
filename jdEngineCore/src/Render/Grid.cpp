#include <glm/glm.hpp>
#include "Render/Grid.h"
#include "Render/ShaderComponent.h"
#include "Render/UniformBuffer.h"
#include "Render/VertexBuffer.h"
#include "ECS/jdEngine.h"
#include "Utils/logger.h"
#include <array>

Grid::Grid()
{
	auto& engine = jd::Engine::getEngine();
	auto& shaderManager = engine.getShaderManager();

	ShaderComponent vertex, fragment;
	vertex.loadComponentFromFile(ROOT_DIR "res/grid/grid.vert", GL_VERTEX_SHADER);
	fragment.loadComponentFromFile(ROOT_DIR "res/grid/grid.frag", GL_FRAGMENT_SHADER);

	auto& shader = shaderManager.createShaderProgram("grid_ui");
	shader.addComponentToProgram(vertex);
	shader.addComponentToProgram(fragment);
	shader.linkProgram();
	// Matrices need to be establish in maintain render loop
	shader.use();
	UniformBuffer::bindShaderUniformBlock(shader.getId(), UniformBlockBindingPoints::MATRICES, "Matrices");

	std::array<glm::vec3, 2> vert{{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) }};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	VertexBuffer vb;
	vb.makeVBO(sizeof(glm::vec3) * vert.size());
	vb.bindVBO(GL_ARRAY_BUFFER);
	vb.addRawData(vert.data(), sizeof(glm::vec3) * vert.size());
	vb.uploadDataToGPU(GL_STATIC_DRAW, true);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glBindVertexArray(0);

	LOG(INFO) << "creating Grid on VAO id=" << VAO;
}

void Grid::Draw()
{
	auto& engine = jd::Engine::getEngine();
	auto& shaderManager = engine.getShaderManager();
	auto& shader = shaderManager.getShaderProgram("grid_ui");

	shader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 2 * sizeof(float));
}

Grid::~Grid() noexcept
{
	auto& engine = jd::Engine::getEngine();
	auto& shaderManager = engine.getShaderManager();

	LOG(INFO) << "deleating Grid on VAO id=" << VAO;
	glDeleteVertexArrays(1, &VAO);
	shaderManager.deleteShaderProgram("grid_ui");
}