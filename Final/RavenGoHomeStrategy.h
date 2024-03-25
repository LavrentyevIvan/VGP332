#pragma once

#include <AI.h>

#include "Raven.h"
#include "GoalMoveToPosition.h"
class RavenGoHomeStrategy : public AI::Strategy<Raven>
{
public:
	float CalculateDesirability(Raven& agent) const override;
	std::unique_ptr<AI::Goal<Raven>> CreateGoal() const override;

private:
	const X::Math::Vector2 home = { 50,50 };
}; 
