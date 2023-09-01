#pragma once
#include "EventComponent.h"

class EntityCreationEvent : public EventComponent
{
public:
	EntityCreationEvent(Entity _entity) :
		EventComponent("EntityCreationEvent", Events::Creation::ENTITY),
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

class EntityTransformEvent : public EventComponent
{
public:
	EntityTransformEvent(Entity _entity) :
		EventComponent("EntityTransformEvent", Events::Creation::TRANSFORM),
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