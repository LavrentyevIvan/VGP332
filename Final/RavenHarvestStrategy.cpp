#include "RavenHarvestStrategy.h"

#include "GoalHarvestMineral.h"
#include "TypeIds.h"

float RavenHarvestStrategy::CalculateDesirability(Raven& agent) const
{
	if (agent.getState() == ravenStates::HarvestMushroom)
	{
		Mineral* mineral = nullptr;
		AI::EntityPtrs minerals = agent.world.GetEntitiesInRAnge({ agent.destination, 1.0f }, static_cast<uint32_t>(AgentType::Mineral));
		if (!minerals.empty())
		{
			mineral = static_cast<Mineral*>(minerals[0]);
		}

		if (mineral != nullptr && mineral->GetHealth() > 0)
		{
			const Mineral* mineral = (const Mineral*)(agent.target);
			if (X::Math::DistanceSqr(agent.position, agent.destination) < 300.0f)
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