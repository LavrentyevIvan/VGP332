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

void AIWorld::AddObstacle(const X::Math::Circle& obstacle)
{
	mObstacles.push_back(obstacle);
}

void AIWorld::AddWall(const X::Math::LineSegment& wall)
{
	mWalls.push_back(wall);
}

bool AI::AIWorld::HasLineOfSight(const X::Math::LineSegment& lineSegment) const
{
	for (const X::Math::LineSegment& wall : mWalls)
	{
		if (X::Math::Intersect(lineSegment, wall))
		{
			return false;

		}
	}
	for (const X::Math::Circle& obstacle : mObstacles)
	{
		if (X::Math::Intersect(lineSegment, obstacle))
		{
			return false;

		}
	}
	return true;
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

