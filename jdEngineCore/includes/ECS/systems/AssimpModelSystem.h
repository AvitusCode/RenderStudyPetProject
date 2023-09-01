#pragma once
#include "../System.h"
#include <memory>
/*
Require components: 

AssimpModelComponent
Rendereble
Transform

*/

class EventComponent;
struct AssimpModel;

class AssimpModelSystem : public System
{
public:
	static std::shared_ptr<AssimpModelSystem> getSystem();

	void OnInit() override;
	void OnUpdate(float dt) override;
	void OnDispose() override;

private:
	void updateEntity(Entity entity, AssimpModel& assimpModel);
	void updateEntityFirst(Entity entity, AssimpModel& assimpModel);
	void ModelAddListener(EventComponent* event);
};