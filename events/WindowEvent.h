#pragma once
#include "EventComponent.h"


class WindowResizedEvent : public EventComponent
{
public:
	WindowResizedEvent(int width, int height) :
		EventComponent("Window resize event", Events::Window::RESIZED),
		m_width(width),
		m_height(height) {}

	std::string format() const override {
		return m_name + ": " + std::to_string(m_width) + ", " + std::to_string(m_height);
	}

	int getWidth() const {
		return m_width;
	}

	int getHeight() const {
		return m_height;
	}

private:
	int m_width, m_height;
};

class WindowClosedEvent : public EventComponent
{
public:
	WindowClosedEvent() : EventComponent("Window Close Event", Events::Window::QUIT) {}
	
	std::string format() const override {
		return m_name;
	}
};