#include "Precompiled.h"
#include "Entity.h"

#include "AIWorld.h";

using namespace AI;

AI::Entity::Entity(AIWorld& world, uint32_t typeId)
	:world(world)
	,mUniqueId(static_cast<uint64_t>(typeId) << 32 | world.GetNextId())
{
	world.Register(this);

}

AI::Entity::~Entity()
{
	world.Unregister(this);
}

X::Math::Matrix3 AI::Entity::GetWorldTransform() const
{
	const X::Math::Vector2& h = heading;
	const X::Math::Vector2 s = X::Math::PerpendicularRH(h);
	const X::Math::Vector2& d = position;


	return {
		s.x, s.y, 0.0f,
		h.x, h.y, 0.0f,
		d.x, d.y, 1.0f

	};
}
