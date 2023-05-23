#pragma once
#include "EventComponent.h"

class Body2DCreationEvent : public EventComponent
{
public:
	Body2DCreationEvent(Entity _entity) : 
		EventComponent("Body2DCreationEvent", Events::Creation::BODY2D),
		m_entity(_entity) {}

	std::string format() const override {
		return m_name + ", EntityID( " + std::to_string(m_entity) + ")";
	}

	Entity getEntity() const {
		return m_entity;
	}
private:
	Entity m_entity;
};