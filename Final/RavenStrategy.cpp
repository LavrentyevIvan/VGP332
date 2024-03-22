#include "RavenStrategy.h"
#include "GoalMoveToPosition.h"

void RavenStrategy::SetTargetDestination(const X::Math::Vector2& destination)
{
	mTargetDestination = destination;
}

float RavenStrategy::CalculateDesirability(Raven& agent) const
{
	return 100.0f;
}

std::unique_ptr<AI::Goal<Raven>> RavenStrategy::CreateGoal() const
{
	auto newGoal = std::make_unique<GoalMoveToPosition>();
	newGoal->SetDestination(mTargetDestination);
	return newGoal;
}