#pragma once
#include "../System.h"
#include <memory>

class Model2DSystem : public System
{
public:
	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

	static std::shared_ptr<Model2DSystem> getSystem();
private:
};