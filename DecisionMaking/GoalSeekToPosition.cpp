#include "GoalSeekToPosition.h"

GoalSeekToPosition::GoalSeekToPosition()
{
}

void GoalSeekToPosition::Activate(Raven& agent)
{
	mStatus = GoalSeekToPosition::Status::Active;
	agent.SetSeek(true);
	agent.destination = mDestination;
}

GoalSeekToPosition::Status GoalSeekToPosition::Process(Raven& agent)
{
	ActivateIfInactive(agent);
	if (X::Math::DistanceSqr(agent.position, mDestination) < mDestinationRange * mDestinationRange)
	{
		mStatus = GoalSeekToPosition::Status::Completed;
	}
	return mStatus;
}

void GoalSeekToPosition::Terminate(Raven& agent)
{
	agent.SetSeek(false);
}

void GoalSeekToPosition::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}

void GoalSeekToPosition::SetDestinationRange(float range)
{
	mDestinationRange = range;
}