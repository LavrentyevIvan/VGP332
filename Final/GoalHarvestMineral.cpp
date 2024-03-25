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
	Mineral* mineral = nullptr;
	AI::EntityPtrs minerals = agent.world.GetEntitiesInRAnge({ agent.destination, 1.0f }, static_cast<uint32_t>(AgentType::Mineral));
	if (!minerals.empty())
	{
		mineral = static_cast<Mineral*>(minerals[0]);
	}

	if (mineral == nullptr)
	{
		mStatus = GoalHarvestMineral::Status::Failed;
	}
	else if (mStartTime + 5.0f < X::GetTime())
	{

		mineral->SetHealth(0);
		agent.target = nullptr;
		agent.CollectMushroom();
		agent.setHarvested(true);
		mStatus = GoalHarvestMineral::Status::Completed;

	}
	return mStatus;
}

void GoalHarvestMineral::Terminate(Raven& agent)
{

}