#pragma once

#include "configurations.h"
#include <set>


class System
{
public:
	virtual ~System() = default;

	virtual void OnInit() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnDispose() = 0;

	std::set<Entity> mEntities;
};
