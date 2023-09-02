#pragma once

#include "../System.h"
#include "Render/Shader.h"
#include "Render/UniformBuffer.h"
#include "Render/Primitives.h"
#include "Render/Grid.h"
#include <memory>

class EventComponent;
class Texture;

// A first iteration of Render piplene, will be improve in next iterations
class RenderSystem : public System
{
public:
	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

	static std::shared_ptr<RenderSystem> getSystem();

private:
	void resizeListener(EventComponent*);
	void setTextures(const GLuint shaderId, const std::vector<const Texture*>& textures);

	Entity mCamera;
	Primitives::Frustum frustum;
	UniformBuffer UBO;
	Grid* grid;
};
