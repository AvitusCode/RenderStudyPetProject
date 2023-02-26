#pragma once
#include "Event.h"

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(int _x, int _y) :
		Event("MouseMovedEvent", Event::EventType::MOUSE_MOVE),
		x(_x),
		y(_y) {}

	std::string format() const override {
		return m_name + ": " + std::to_string(x) + ", " + std::to_string(y);
	}

	int getX() const { 
		return x; 
	}

	int getY() const {
		return y;
	}

private:
	int x, y;
};

class MouseScrolledEvent : public Event
{

public:
	MouseScrolledEvent(float offset) :
		Event("MouseScrolledEvent", Event::EventType::MOUSE_SCROLL),
		m_offset(offset) {}

	std::string format() const override {
		return (m_offset < 0) ? m_name + ": " + "to bottom" : m_name + ": " + "to top";
	}

	float getOffset() const {
		return m_offset;
	}

private:
	float m_offset;
};