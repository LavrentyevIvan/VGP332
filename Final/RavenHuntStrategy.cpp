#include "RavenHuntStrategy.h"

#include "GoalMoveToPosition.h"


float RavenHuntStrategy::CalculateDesirability(Raven& agent) const
{
	if (agent.getState() == ravenStates::SearchForMushroom)
	{
	return 10000.0f;

	}
	else 
	{
	return 0.0f;

	}
}

std::unique_ptr<AI::Goal<Raven>> RavenHuntStrategy::CreateGoal() const
{
	
	auto newGoal = std::make_unique<GoalMoveToPosition>();

	newGoal->SetDestination(X::Math::Vector2{ 550.0f,400.0f });

	return newGoal;
}