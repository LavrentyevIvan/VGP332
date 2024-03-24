#pragma once

#include <AI.h>
#include "Raven.h"
#include "TypeIds.h"
class GoHome : public AI::State<Raven> {
public:
	void Enter(Raven& agent) override;
	void Update(Raven& agent, float deltaTime) override;
	void Exit(Raven& agent) override;
	void DebugUI() override;
};
class SearchForMushroom : public AI::State<Raven> {
public:

	void Enter(Raven& agent) override;
	void Update(Raven& agent, float deltaTime) override;
	void Exit(Raven& agent) override;
	void DebugUI() override;
	void setPerception(const AI::PerceptionModule* eyes );
private:
	const AI::PerceptionModule* mPerception;
};
class MoveToMushroom : public AI::State<Raven> {
public:
	void Enter(Raven& agent) override;
	void Update(Raven& agent, float deltaTime) override;
	void Exit(Raven& agent) override;
	void DebugUI() override;
};
class HarvestMushroom : public AI::State<Raven> {
public:
	void Enter(Raven& agent) override;
	void Update(Raven& agent, float deltaTime) override;
	void Exit(Raven& agent) override;
	void DebugUI() override;
};

