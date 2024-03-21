#include "RavenHarvestStrategy.h"

#include "GoalHarvestMineral.h"

float RavenHarvestStrategy::CalculateDesirability(Raven& agent) const
{
	if (agent.GetTarget() != nullptr)
	{
		float distanceSqr = X::Math::DistanceSqr(agent.position, agent.GetTarget()->position);
		Mineral* mineral = static_cast<Mineral*>(agent.GetTarget());
		if (distanceSqr < 25.0f && mineral->GetHealth() > 0)
		{
			return 1000.0f;
		}
	}
	return 0.0f;
}

std::unique_ptr<AI::Goal<Raven>> RavenHarvestStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalHarvestMineral>();
	return newGoal;
}