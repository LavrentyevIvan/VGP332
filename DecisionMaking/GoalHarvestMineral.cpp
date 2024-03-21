#include "GoalHarvestMineral.h"

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
	Mineral* mineral = static_cast<Mineral*>(agent.GetTarget());
	mineral->SetHealth(mineral->GetHealth() - (20 * deltaTime));
	if (mineral->GetHealth() < 0)
	{
		agent.SetTarget(nullptr);
		mStatus = GoalHarvestMineral::Status::Completed;
	}
	return mStatus;
}

void GoalHarvestMineral::Terminate(Raven& agent)
{

}