#include "GoalHarvestMineral.h"
#include "TypeIds.h"

GoalHarvestMineral::GoalHarvestMineral()
{
}

void GoalHarvestMineral::Activate(Raven& agent)
{
	mStartTime = X::GetTime();
}

GoalHarvestMineral::Status GoalHarvestMineral::Process(Raven& agent)
{
	ActivateIfInactive(agent);

	float currentTime = X::GetTime();
	float deltaTime = currentTime - mStartTime + 0.01f;
	mStartTime = currentTime;
	auto entities = agent.world.GetEntitiesInRAnge({ agent.destination, 1.0f }, static_cast<uint32_t>(AgentType::Mineral));
	if (!entities.empty())
	{
		Mineral* mineral = static_cast<Mineral*>(entities[0]);
		mineral->SetHealth(mineral->GetHealth() - (20 * deltaTime));
		if (mineral->GetHealth() < 0)
		{
			agent.SetTarget(nullptr);
			agent.CollectMushroom();
			agent.setHarvested(true);
			mStatus = GoalHarvestMineral::Status::Completed;
		}
	}
	return mStatus;
}

void GoalHarvestMineral::Terminate(Raven& agent)
{

}