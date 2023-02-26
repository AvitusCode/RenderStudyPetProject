#pragma once
#include <string>

class Event
{
public:
	enum class EventType : int8_t
	{
		MOUSE_SCROLL = 1,
		MOUSE_MOVE,
		MOUSE_BUTTON,
		KEY_PRESSED,
		WINDOW_RESIZE,
		WINDOW_CLOSE
	};

	inline const std::string& getName() const { return m_name; }
	inline EventType getType() const { return m_type; }
	virtual std::string format() const = 0;

	virtual ~Event() = default;

protected:
	Event(std::string name, EventType type) : m_name(std::move(name)), m_type(type) {}

	std::string m_name;
	EventType m_type;
};