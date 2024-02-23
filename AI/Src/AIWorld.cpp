#include "Precompiled.h"
#include "AIWorld.h"
using namespace AI;

void AIWorld::Initialize()
{

}

void AIWorld::Update()
{

}

void AIWorld::Register(Entity* entity)
{
	XASSERT(std::find(mEntities.begin(), mEntities.end(), entity) == mEntities.end(),
		"AIWorld: entity is already added");
	mEntities.push_back(entity);
}

void AIWorld::Unregister(Entity* entity)
{
	auto iter = std::find(mEntities.begin(), mEntities.end(), entity);
	if (iter != mEntities.end()) {
		mEntities.erase(iter);
	}
}

EntityPtrs AIWorld::GetEntitiesInRAnge(const X::Math::Circle& range, uint32_t typeId)
{
	const float radiusSq = range.radius * range.radius;
	EntityPtrs entities;

	for (auto& entity : mEntities)
	{
		if (typeId == 0 || entity->GetTypeId() == typeId)
		{
			const X::Math::Vector2 distVec = entity->position - range.center;
			if (X::Math::MagnitudeSqr(distVec) <= radiusSq)
			{
				entities.push_back(entity);
			}
		}
	}

	return entities;
}

