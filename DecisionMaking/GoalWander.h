#pragma once

#include <AI.h>

#include "Raven.h"

class GoalWander : public AI::Goal<Raven>
{
public:
	using Status = AI::Goal<Raven>::Status;

	GoalWander();

	void Activate(Raven& agent) override;
	Status Process(Raven& agent) override;
	void Terminate(Raven& agent) override;
};