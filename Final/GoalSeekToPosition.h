#pragma once

#include <AI.h>

#include "Raven.h"

class GoalSeekToPosition : public AI::Goal<Raven>
{
public:
	using Status = AI::Goal<Raven>::Status;

	GoalSeekToPosition();

	void Activate(Raven& agent) override;
	Status Process(Raven& agent) override;
	void Terminate(Raven& agent) override;

	void SetDestination(const X::Math::Vector2& destination);
	void SetDestinationRange(float range);

private:
	X::Math::Vector2 mDestination;
	float mDestinationRange = 10.0f;
};