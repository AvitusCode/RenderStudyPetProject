#include "ECS/EventManager.h"

void EventManager::AddListener(EventId eventId, std::function<void(EventComponent*)> const& listener) {
	listeners[eventId].push_front(listener);
}

void EventManager::SendEvent(EventComponent* event)
{
	EventId type = event->getType();

	for (const auto& listener : listeners[type]) {
		listener(event);
	}
}