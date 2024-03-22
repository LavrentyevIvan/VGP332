#pragma once

#include <AI.h>

#include "Raven.h"

class RavenStrategy : public AI::Strategy<Raven>
{
public:
	void SetTargetDestination(const X::Math::Vector2& destination);

	float CalculateDesirability(Raven& agent) const override;
	std::unique_ptr<AI::Goal<Raven>> CreateGoal() const override;

private:
	X::Math::Vector2 mTargetDestination;
};
