#pragma once
#include "../System.h"
#include <memory>

class MaterialSystem : public System
{
public:
	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

	static std::shared_ptr<MaterialSystem> getSystem();
private:
};