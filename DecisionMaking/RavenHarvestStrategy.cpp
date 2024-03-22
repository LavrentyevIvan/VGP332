#include "RavenHarvestStrategy.h"

#include "GoalHarvestMineral.h"
#include "TypeIds.h"

float RavenHarvestStrategy::CalculateDesirability(Raven& agent) const
{
	if (agent.GetTarget() != nullptr)
	{
		auto entities = agent.world.GetEntitiesInRAnge({ agent.destination, 1.0f }, static_cast<uint32_t>(AgentType::Mineral));
		if (!entities.empty())
		{
			Mineral* mineral = static_cast<Mineral*>(entities[0]);
			float distanceSqr = X::Math::DistanceSqr(agent.position, agent.destination);
			if (distanceSqr < 25.0f && mineral->GetHealth() > 0)
			{
				return 1000.0f;
			}
		}
	}
	return 0.0f;
}

std::unique_ptr<AI::Goal<Raven>> RavenHarvestStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalHarvestMineral>();
	return newGoal;
}