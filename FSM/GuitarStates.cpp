#include "GuitarStates.h"
#include <ImGui/Inc/imgui.h>

void TakeABreakState::Enter(Guitar& agent)
{
	agent.SetLocation(Location::Home);
}

void TakeABreakState::Update(Guitar& agent, float deltaTime)
{
	if (!agent.IsRested()) {
		if (!agent.IsFamous()) 
		{
			agent.ChangeState(GuitarState::HostAConcert);
		}
		else 
		{
			// they are famous now
		}
	}
}

void TakeABreakState::Exit(Guitar& agent)
{

}

void TakeABreakState::DebugUI()
{
	ImGui::Text("TakeABreakState: Rockstar is having a smoke");
}

void HostAConcertState::Enter(Guitar& agent)
{
	agent.SetLocation(Location::Stage);

}

void HostAConcertState::Update(Guitar& agent, float deltaTime)
{
	agent.IncreaseFatigue();
	agent.AddFansGained(100);
	if (agent.IsCapacityReached()) {
		agent.ChangeState(GuitarState::PostOnSocials);
	}
	else if (agent.IsHungry()) {
		agent.ChangeState(GuitarState::VisitRestaraunt);
	}
}

void HostAConcertState::Exit(Guitar& agent)
{

}

void HostAConcertState::DebugUI()
{
	ImGui::Text("HostAConcertState: Rockstar is having a fine concert tonight!");

}

void VisitRestarauntState::Enter(Guitar& agent)
{
	agent.SetLocation(Location::Restaraunt);

}

void VisitRestarauntState::Update(Guitar& agent, float deltaTime)
{
	agent.ResetHunger();
	if (!agent.IsHungry()) {
		agent.ChangeState(GuitarState::HostAConcert);
	}
}

void VisitRestarauntState::Exit(Guitar& agent)
{
}

void VisitRestarauntState::DebugUI()
{
	ImGui::Text("VisitRestarauntState: Rockstar is ravenously devouring a McChicken");

}

void PostOnSocialsState::Enter(Guitar& agent)
{
	agent.SetLocation(Location::SocialMedia);

}

void PostOnSocialsState::Update(Guitar& agent, float deltaTime)
{
	agent.AddFansToSocials();
	if (!agent.IsRested()) 
	{
		agent.ChangeState(GuitarState::TakeABreak);
	}
	else 
	{
		agent.ChangeState(GuitarState::HostAConcert);

	}
}

void PostOnSocialsState::Exit(Guitar& agent)
{
}

void PostOnSocialsState::DebugUI()
{
	ImGui::Text("VisitRestarauntState: Rockstar is gaining fans on their socials");

}
