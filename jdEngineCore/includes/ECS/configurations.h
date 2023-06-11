#pragma once
#include <bitset>
#include <cstdint>
#include "Types.h"

// Source: https://gist.github.com/Lee-R/3839813
constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count) {
	return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
}

constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count) {
	return fnv1a_32(s, count);
}


// ECS CONFIGURATIONS
//// CORE
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;
//// EVENTS
using EventId = std::uint32_t;
using ParamId = std::uint32_t;

// Input
enum class InputButtons
{
	W,
	A,
	S,
	D,
	Q,
	E
};

// event

#define METHOD_LISTENER(EventType, Listener) EventType, std::bind(&Listener, this, std::placeholders::_1)
#define FUNCTION_LISTENER(EventType, Listener) EventType, std::bind(&Listener, std::placeholders::_1)

namespace Events::Window {
const EventId QUIT = "Events::Window::QUIT"_hash;
const EventId RESIZED = "Events::Window::RESIZED"_hash;
const EventId INPUT = "Events::Window::INPUT"_hash;
const EventId SCROLL = "Events::Window::SCROLL"_hash;
const EventId MOUSE = "Events::Window::MOUSE"_hash;
}

namespace Events::Creation {
	const EventId BODY2D = "Events::Creation::BODY2D"_hash;
}