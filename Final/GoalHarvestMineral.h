#pragma once

#include <AI.h>

#include "Raven.h"
#include "Mineral.h"

class GoalHarvestMineral : public AI::Goal<Raven>
{
public:
	using Status = AI::Goal<Raven>::Status;

	GoalHarvestMineral();

	void Activate(Raven& agent) override;
	Status Process(Raven& agent) override;
	void Terminate(Raven& agent) override;

private:
	float mStartTime = 0.0f;
	float mHarvestTime = 5.0f;
	Mineral* mTargetMineral = nullptr;
};