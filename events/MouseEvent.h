#pragma once
#include "EventComponent.h"

class MouseMovedEvent : public EventComponent
{
public:
	MouseMovedEvent(float _x, float _y) :
		EventComponent("MouseMovedEvent", Events::Window::MOUSE),
		x(_x),
		y(_y) {}

	std::string format() const override {
		return m_name + ": " + std::to_string(x) + ", " + std::to_string(y);
	}

	float getX() const { 
		return x; 
	}

	float getY() const {
		return y;
	}

private:
	float x, y;
};

class MouseScrolledEvent : public EventComponent
{

public:
	MouseScrolledEvent(float offset) :
		EventComponent("MouseScrolledEvent", Events::Window::SCROLL),
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