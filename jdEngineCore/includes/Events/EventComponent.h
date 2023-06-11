#pragma once
#include <string>
#include "ECS/configurations.h"

class EventComponent
{
public:
	inline const std::string& getName() const { return m_name; }
	inline EventId getType() const { return m_type; }
	virtual std::string format() const = 0;

	virtual ~EventComponent() = default;

protected:
	EventComponent(std::string name, EventId type) : m_name(std::move(name)), m_type(type) {}

	std::string m_name;
	EventId m_type;
};