#include "VisualSensor.h"
#include "Mineral.h"
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
		X::Math::LineSegment lineToTarget(agent.position, entity->position);
		if (!agent.world.HasLineOfSight(lineToTarget))
		{
			X::DrawScreenLine(agent.position, entity->position, X::Colors::Red);
			continue;

		}

		//have I seen this before
		auto iter = std::find_if(memory.begin(), memory.end(),
			[&](const MemoryRecord& m)
			{
				return entity->GetUniqueId() == m.uniqueId;
			});

		if (iter != memory.end())
		{
			iter->properties["lastSeenPosition"] = entity->position;
			iter->lastRecordedTime = X::GetTime();
			if (entity->GetTypeId() == static_cast<uint32_t>(AgentType::Mineral))
			{
				const Mineral* mineral = static_cast<const Mineral*>(entity);
				iter->properties["health"] = mineral->GetHealth();
			}
		}
		else
		{
			auto& newRecord = memory.emplace_back();
			newRecord.uniqueId = entity->GetUniqueId();
			newRecord.properties["lastSeenPosition"] = entity->position;
			newRecord.properties["type"] = static_cast<int>(entity->GetTypeId());
			newRecord.lastRecordedTime = X::GetTime();
			if (entity->GetTypeId() == static_cast<uint32_t>(AgentType::Mineral))
			{
				const Mineral* mineral = static_cast<const Mineral*>(entity);
				newRecord.properties["health"] = mineral->GetHealth();
			}
		}

	}
	//debug info

	X::Math::Vector2 fovStart = X::Math::Rotate(agent.heading * viewRange, -viewHalfAngle);
	X::Math::Vector2 fovEnd = X::Math::Rotate(agent.heading * viewRange, viewHalfAngle);
	X::DrawScreenLine(agent.position, agent.position + fovStart, X::Colors::Cyan);
	X::DrawScreenLine(agent.position, agent.position + fovEnd, X::Colors::Cyan);

	float angle = atan2(agent.heading.y, agent.heading.x);
	X::DrawScreenArc(agent.position, viewRange, angle - viewHalfAngle, angle + viewHalfAngle, X::Colors::Cyan);







}