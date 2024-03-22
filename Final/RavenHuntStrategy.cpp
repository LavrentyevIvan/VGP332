#include "RavenHuntStrategy.h"

#include "GoalWander.h"


float RavenHuntStrategy::CalculateDesirability(Raven& agent) const
{
	return 10.0f;
}

std::unique_ptr<AI::Goal<Raven>> RavenHuntStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalWander>();
	return newGoal;
}