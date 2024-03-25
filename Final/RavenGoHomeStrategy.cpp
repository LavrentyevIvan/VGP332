#include "RavenGoHomeStrategy.h"

float RavenGoHomeStrategy::CalculateDesirability(Raven& agent) const
{
    if (agent.getState() == ravenStates::GoHome)
    {
        return 10000.0f;
    }
    else 
    {
        return 0.0f;

    }
}
std::unique_ptr<AI::Goal<Raven>> RavenGoHomeStrategy::CreateGoal() const 
{
    auto newGoal = std::make_unique<GoalMoveToPosition>();

    newGoal->SetDestination(home);

    return newGoal;
}