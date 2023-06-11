#pragma once
#include "../System.h"
#include <memory>

class ModelSystem2D : public System
{
public:
	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

	static std::shared_ptr<ModelSystem2D> getModelSystem();
private:
};