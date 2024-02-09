#include "Guitar.h"
#include <ImGui/Inc/imgui.h>
#include "GuitarStates.h"
void Guitar::Initialize()
{
	gLocation = Location::Home;
	gFansGained = 0;
	gSocialsFans = 0;
	gHunger = 0;
	gFatigue = 0;

	gStateMachine.Initialize(this);
	gStateMachine.AddState<TakeABreakState>();
	gStateMachine.AddState<HostAConcertState>();
	gStateMachine.AddState<VisitRestarauntState>();
	gStateMachine.AddState<PostOnSocialsState>();
	ChangeState(GuitarState::TakeABreak);

}

void Guitar::Terminate()
{
	gStateMachine.Terminate();
}

void Guitar::Update(float deltaTime)
{
	gStateMachine.Update(deltaTime);

}

void Guitar::ChangeState(GuitarState newState)
{
	gStateMachine.ChangeState((int)newState);
}

void Guitar::DebugUI()
{
	gStateMachine.DebugUI();
	ImGui::Text("Status: SocialFans[%d] Fans[%d] Hunger[%d] Fatigue[%d]",
		gSocialsFans, gFansGained, gHunger, gFatigue);
}

Location Guitar::GetLocation() const
{
	return gLocation;
}

bool Guitar::IsHungry() const
{
	return gHunger > 5;
}

bool Guitar::IsCapacityReached() const
{
	return gFansGained > 1000;
}

bool Guitar::IsFamous() const
{
	return gSocialsFans > 5000;
}

bool Guitar::IsRested() const
{
	return gFatigue == 0;
}

void Guitar::SetLocation(Location location)
{
	gLocation = location;
}

void Guitar::IncreaseFatigue()
{
	++gFatigue;
	++gHunger;
}

void Guitar::AddFansGained(int amount)
{
	 gFansGained +=amount;
	
}

void Guitar::AddFansToSocials()
{
	gSocialsFans += gFansGained;
	gFansGained = 0;
}

void Guitar::ResetHunger()
{
	gHunger = 0;
}

void Guitar::ResetRest()
{
	gFatigue = 0;
}
