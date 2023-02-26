#pragma once
#include "Event.h"

class KeyPressedEvent : public Event
{
public:
	KeyPressedEvent(int key, int action) : 
		Event("KeyPressedEvent", Event::EventType::KEY_PRESSED),
		m_key(key),
		m_action(action) {}

	std::string format() const override {
		return m_name + ": " + "Key[" + std::to_string(m_key) + "], Action[" + std::to_string(m_action) + "]";
	}

	int getKey() const {
		return m_key;
	}

	int getAction() const {
		return m_action;
	}

private:
	int m_key, m_action;
};