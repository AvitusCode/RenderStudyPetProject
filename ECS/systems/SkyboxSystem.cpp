#include "SkyboxSystem.h"
#include "../jdEngine.h"
#include "../components/SkyboxComponent.h"
#include "../../render/Shader.h"
#include "../../render/UniformBuffer.h"
#include "../../GL_ERROR.h"

void SkyboxSystem::OnInit()
{
	// only one skybox in current rende time
	ShaderComponent vertexShader, fragmentShader;
	vertexShader.loadComponentFromFile("res/skybox/skybox.vert", GL_VERTEX_SHADER);
	fragmentShader.loadComponentFromFile("res/skybox/skybox.frag", GL_FRAGMENT_SHADER);

	skyShader = new Shader{ vertexShader, fragmentShader };
	
	skyShader->use();
	skyShader->setInt("skybox", 1);

	UniformBuffer::bindShaderUniformBlock(skyShader->getId(), UniformBlockBindingPoints::MATRICES, "Matrices");
}

void SkyboxSystem::OnUpdate(float dt)
{
	// only one skybox in current rende time
	auto& jd_engine = jd::Engine::getEngine();
	Entity skybox = *mEntities.begin();

	const auto& skyComponent = jd_engine.GetComponent<SkyboxComponent>(skybox);

	skyShader->use();

	glDepthFunc(GL_LEQUAL);
	
	glBindVertexArray(skyComponent.VAO);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyComponent.texId);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}

void SkyboxSystem::OnDispose()
{
	auto& jd_engine = jd::Engine::getEngine();
	Entity skybox = *mEntities.begin();

	const auto& skyComponent = jd_engine.GetComponent<SkyboxComponent>(skybox);

	glDeleteVertexArrays(1, &skyComponent.VAO);
	glDeleteTextures(1, &skyComponent.texId);

	delete skyShader;
}