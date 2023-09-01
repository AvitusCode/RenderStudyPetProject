#pragma once
#include "ComponentArray.h"
#include "configurations.h"
#include <any>
#include <memory>
#include <unordered_map>

#include <iostream>

class ComponentManager
{
public:
	template<typename T>
	ComponentType RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		if (auto& component = mComponentTypes.find(typeName); component != mComponentTypes.end()) {
			return component->second;
		}

		mComponentTypes.insert({typeName, mNextComponentType});
		mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

		++mNextComponentType;
		return mComponentTypes[typeName];
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		return mComponentTypes[typeName];
	}

	template<typename T>
	void AddComponent(Entity entity, T&& component) {
		GetComponentArray<std::remove_reference_t<T>>()->InsertData(entity, std::move(component));
	}

	template<typename T>
	void RemoveComponent(Entity entity) {
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity) {
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity)
	{
		for (const auto& [pair, component] : mComponentArrays) {
			component->EntityDestroyed(entity);
		}
	}

private:
	std::unordered_map<const char*, ComponentType> mComponentTypes{};
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};
	ComponentType mNextComponentType{};

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}
};
