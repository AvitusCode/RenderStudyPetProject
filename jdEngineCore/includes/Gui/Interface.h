#pragma once
#include <variant>
#include <optional>
#include <string>

namespace sm::model
{
	namespace state
	{
		struct Idle {};
		struct Selectable { bool calcTangentSpace{ false }; bool getSmoothNormal{ false }; };
		struct Work {};
	}

	namespace event
	{
		struct Add {};
		struct Choose {};
		struct Draw {};
	}

	using State = std::variant<state::Idle, state::Selectable, state::Work>;
	using Event = std::variant<std::monostate, event::Add, event::Choose, event::Draw>;
	using MaybeState = std::optional<State>;
}

namespace jd
{
	class ModelMenu final
	{
	public:
		ModelMenu() = default;
		void processEvent(const sm::model::Event& event);
		static std::string info() {
			return "sm::model::ModelMenu";
		}
	private:
		sm::model::State state_;
	};

	class Interface final
	{
	public:
		// Performance info : framerate etc.
		static void StatusMenu();
		static void ProgMenu();

	private:
		inline static ModelMenu modelMenu{};
	};
}