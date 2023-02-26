#pragma once
#include "Event.h"


class WindowResizedEvent : public Event
{
public:
	WindowResizedEvent(int width, int height) :
		Event("Window resize event", Event::EventType::WINDOW_RESIZE),
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

class WindowClosedEvent : public Event
{
public:
	WindowClosedEvent() : Event("Window Close Event", Event::EventType::WINDOW_CLOSE) {}
	
	std::string format() const override {
		return m_name;
	}
};