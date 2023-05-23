#pragma once

#include "../System.h"
#include "../../render/Shader.h"
#include "../../render/UniformBuffer.h"
#include "../../render/Primitives.h"
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

	static std::shared_ptr<RenderSystem> getRenderSystem();

private:
	void resizeListener(EventComponent*);
	void setTextures(const GLuint shaderId, const std::vector<const Texture*>& textures);

	Entity mCamera;
	Primitives::Frustum frustum;
	UniformBuffer UBO;
};
