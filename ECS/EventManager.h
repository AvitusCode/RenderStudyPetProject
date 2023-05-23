#pragma once
#include "../Events/EventComponent.h"
#include "configurations.h"
#include <functional>
#include <forward_list>
#include <unordered_map>


class EventManager
{
public:
	void AddListener(EventId eventId, std::function<void(EventComponent*)> const& listener);
	void SendEvent(EventComponent* event);

private:
	std::unordered_map<EventId, std::forward_list<std::function<void(EventComponent*)>>> listeners;
};
