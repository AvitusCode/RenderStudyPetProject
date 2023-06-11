#include "ECS/SystemManager.h"

void SystemManager::EntityDestroyed(Entity entity)
{
	for (auto const& [type, system] : mSystems) {
		system->mEntities.erase(entity);
	}
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
{
	for (auto const& [type, system] : mSystems)
	{
		auto const& systemSignature = mSignatures[type];

		if ((entitySignature & systemSignature) == systemSignature) {
			system->mEntities.insert(entity);
		}
		else {
			system->mEntities.erase(entity);
		}
	}
}