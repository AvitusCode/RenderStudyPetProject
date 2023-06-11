#pragma once
#include "../System.h"
#include <memory>
/*
Require components: 

AssimpModelComponent
Rendereble
Transform

*/
class AssimpModelSystem : public System
{
public:
	static std::shared_ptr<AssimpModelSystem> getAssimpModelSystem();

	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

private:

};