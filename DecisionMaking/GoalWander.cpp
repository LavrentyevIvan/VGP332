#include "GoalWander.h"

GoalWander::GoalWander()
{
}

void GoalWander::Activate(Raven& agent)
{
	mStatus = GoalWander::Status::Active;
	agent.SetWander(true);
}

GoalWander::Status GoalWander::Process(Raven& agent)
{
	ActivateIfInactive(agent);
	return mStatus;
}

void GoalWander::Terminate(Raven& agent)
{
	agent.SetWander(false);
}