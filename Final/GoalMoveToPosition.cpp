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
	//mDestination = agent.ravenTilemap->GetTilePosition(X::Math::Vector2{200,200});

	Path path = agent.ravenTilemap->FindPathAStar(startPos.x, startPos.y, mDestination.x, mDestination.y);

	for (int i = 0; i < path.size(); ++i)
	{
		if (i < path.size() - 1) {
		GoalSeekToPosition* seek = AddSubGoal<GoalSeekToPosition>();
		seek->SetDestination(path[i]);
		}
		else
		{
			GoalArriveAtPosition* arrive = AddSubGoal<GoalArriveAtPosition>();
			arrive->SetDestination(path[i]);
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
}

void GoalMoveToPosition::SetDestination(const X::Math::Vector2& destination)
{
	mDestination = destination;
}