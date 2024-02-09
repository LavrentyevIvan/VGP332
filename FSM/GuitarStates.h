#pragma once

#include <AI.h>
#include "Guitar.h"

class TakeABreakState : public AI::State<Guitar> {
public:
	void Enter(Guitar& agent ) override;
	void Update(Guitar& agent, float deltaTime) override;
	void Exit(Guitar& agent ) override;
	void DebugUI() override;
};
class HostAConcertState : public AI::State<Guitar> {
public:
	void Enter(Guitar& agent) override;
	void Update(Guitar& agent, float deltaTime) override;
	void Exit(Guitar& agent) override;
	void DebugUI() override;
};
class VisitRestarauntState : public AI::State<Guitar> {
public:
	void Enter(Guitar& agent) override;
	void Update(Guitar& agent, float deltaTime) override;
	void Exit(Guitar& agent) override;
	void DebugUI() override;
};
class PostOnSocialsState : public AI::State<Guitar> {
public:
	void Enter(Guitar& agent) override;
	void Update(Guitar& agent, float deltaTime) override;
	void Exit(Guitar& agent) override;
	void DebugUI() override;
};
