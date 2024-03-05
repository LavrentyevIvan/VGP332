#include "VisualSensor.h"

using namespace AI;

void VisualSensor::Update(AI::Agent& agent, AI::MemoryRecords& memory, float deltaTime)
{
	const float viewRangeSqr = viewRange * viewRange;
	const AI::EntityPtrs entities = agent.world.GetEntities();
	for (const Entity* entity : entities)
	{
		//ignore self
		if (entity == &agent)
		{
			continue;
		}

		//filter by type
		if (targetType != AgentType::Invalid && static_cast<uint32_t>(targetType) != entity->GetTypeId())
		{
			continue;
		}

		//check if target is in view distance
		X::Math::Vector2 posToTarget = entity->position - agent.position;
		if (X::Math::MagnitudeSqr(posToTarget) > viewRangeSqr)
		{
			continue;
		}
		//check view angle
		X::Math::Vector2 dirToTarget = X::Math::Normalize(posToTarget);
		float dot = X::Math::Dot(agent.heading, dirToTarget);
		float angleToTarget = acos(dot);
		if (angleToTarget > viewHalfAngle)
		{
			continue;
		}


		//Line of Sight

		//have I seen this before
		auto iter = std::find_if(memory.begin(), memory.end(), [](const MemoryRecords& m)
			{
				return entity->GetUniqueId() == m.
			});
	}
}