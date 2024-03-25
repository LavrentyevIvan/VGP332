#include "GoalMoveToPosition.h"
#include "GoalSeekToPosition.h"
#include "GoalArriveAtPosition.h"
GoalMoveToPosition::GoalMoveToPosition()
{
}

void GoalMoveToPosition::Activate(Raven& agent)
{
	float seekDistance = 20.0f;

	mStatus = GoalMoveToPosition::Status::Active;
	RemoveAllSubGoals(agent);

	startPos = agent.ravenTilemap->GetTilePosition(agent.position);
	endPos = agent.ravenTilemap->GetTilePosition(mDestination);
	agent.SetTargetDestination(mDestination);

	Path path = agent.ravenTilemap->FindPathAStar(startPos.x, startPos.y, endPos.x, endPos.y);

	for (int i = 0; i < path.size(); ++i)
	{
		if (i == path.size()) {
			GoalArriveAtPosition* arrive = AddSubGoal<GoalArriveAtPosition>();
			arrive->SetDestination(path[i]);
		}
		else
		{
			
			GoalSeekToPosition* seek = AddSubGoal<GoalSeekToPosition>();
			seek->SetDestination(path[i]);
		}
	}
	

}

GoalMoveToPosition::Status GoalMoveToPosition::Process(Raven& agent)
{
	ActivateIfInactive(agent);
	mStatus = ProcessSubGoals(agent);
	ReactivateIfFailed(agent);
	return mStatus;
}

void GoalMoveToPosition::Terminate(Raven& agent)
{
	RemoveAllSubGoals(agent);
	mStatus = GoalMoveToPosition::Status::Inactive;

}

void GoalMoveToPosition::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}