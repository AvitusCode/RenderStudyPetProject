#pragma once
#include "../System.h"

class MaterialSystem : public System
{
public:
	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

private:
};