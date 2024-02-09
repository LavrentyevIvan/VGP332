#pragma once

#include <AI.h>

enum class Location {
	Home,
	Stage,
	Restaraunt,
	SocialMedia
};

enum class GuitarState {
	TakeABreak,
	HostAConcert,
	VisitRestaraunt,
	PostOnSocials
};

class Guitar {
public:
	Guitar() = default;
	void Initialize();
	void Terminate();
	void Update(float deltaTime);
	void ChangeState(GuitarState newState);
	void DebugUI();

	//check guitar status
	Location GetLocation() const;
	bool IsHungry() const;
	bool IsCapacityReached() const;
	bool IsFamous() const;
	bool IsRested() const;

	//update status functions
	void SetLocation(Location location);
	void IncreaseFatigue();
	void AddFansGained(int amount);
	void AddFansToSocials();

	void ResetHunger();
	void ResetRest();

private:
	AI::StateMachine<Guitar> gStateMachine;
	Location gLocation;
	int gFansGained;
	int gSocialsFans;
	int gHunger;
	int gFatigue;
};