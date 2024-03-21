#pragma once

#include <AI.h>

#include "Raven.h"

class GoalArriveAtPosition : public AI::Goal<Raven>
{
public:
	using Status = AI::Goal<Raven>::Status;

	GoalArriveAtPosition();

	void Activate(Raven& agent) override;
	Status Process(Raven& agent) override;
	void Terminate(Raven& agent) override;

	void SetDestination(const X::Math::Vector2& destination);

private:
	float mDestinatioRange = 10.0f;
	X::Math::Vector2 mDestination;
};