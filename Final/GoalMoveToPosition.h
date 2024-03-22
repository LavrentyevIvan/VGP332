#pragma once

#include <AI.h>

#include "Raven.h"

class GoalMoveToPosition : public AI::GoalComposite<Raven>
{
public:
	using Status = AI::Goal<Raven>::Status;

	GoalMoveToPosition();

	void Activate(Raven& agent) override;
	Status Process(Raven& agent) override;
	void Terminate(Raven& agent) override;

	void SetDestination(const X::Math::Vector2& destination);

private:
	X::Math::Vector2 mDestination;
};