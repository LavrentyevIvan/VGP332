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
