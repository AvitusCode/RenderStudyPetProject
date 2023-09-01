#pragma once
#include "../System.h"
#include <memory>

class FogSystem : public System
{
public:
	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

	static std::shared_ptr<FogSystem> getSystem();
private:
};