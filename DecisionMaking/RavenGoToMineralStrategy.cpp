#include "RavenGoToMineralStrategy.h"

#include "TypeIds.h"
#include "GoalMoveToPosition.h"

void RavenGoToMineralStrategy::SetPerception(const AI::PerceptionModule* perception)
{
	mPerception = perception;
}

float RavenGoToMineralStrategy::CalculateDesirability(Raven& agent) const
{
	const auto& memoryRecords = mPerception->GetMemoryRecords();
	float highestImportance = 0.0f;
	for (auto& record : memoryRecords)
	{
		AgentType agentType = static_cast<AgentType>(record.GetProperty<int>("type", 0));
		if (agentType == AgentType::Mineral)
		{
			if (record.importance > highestImportance)
			{
				highestImportance = record.importance;
				const AI::EntityPtrs entities = agent.world.GetEntities();
				for (AI::Entity* entity : entities)
				{
					if (entity->GetUniqueId() == record.uniqueId)
					{
						agent.SetTarget(entity);
					}
				}
			}
		}
	}
	return highestImportance;
}

std::unique_ptr<AI::Goal<Raven>> RavenGoToMineralStrategy::CreateGoal() const
{
	const auto& memoryRecords = mPerception->GetMemoryRecords();
	float highestImportance = 0.0f;
	X::Math::Vector2 targetDestination = X::Math::Vector2::Zero();
	for (auto& record : memoryRecords)
	{
		AgentType agentType = static_cast<AgentType>(record.GetProperty<int>("type", 0));
		if (agentType == AgentType::Mineral)
		{
			if (record.importance > highestImportance)
			{
				highestImportance = record.importance;
				targetDestination = record.GetProperty<X::Math::Vector2>("lastSeenPosition");
			}
		}
	}
	auto newGoal = std::make_unique<GoalMoveToPosition>();
	newGoal->SetDestination(targetDestination);
	return newGoal;
}