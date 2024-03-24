#include "RavenHuntStrategy.h"

#include "GoalMoveToPosition.h"


float RavenHuntStrategy::CalculateDesirability(Raven& agent) const
{
	if (agent.getState() == ravenStates::SearchForMushroom)
	{
	return 100.0f;

	}
	else 
	{
	return 0.0f;

	}
}

std::unique_ptr<AI::Goal<Raven>> RavenHuntStrategy::CreateGoal() const
{
	
	auto newGoal = std::make_unique<GoalMoveToPosition>();
	newGoal->SetDestination(X::Math::Vector2{ 20,20 });

	return newGoal;
}