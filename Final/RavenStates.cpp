#include "RavenStates.h"
void GoHome::Enter(Raven& agent)
{

}

void GoHome::Update(Raven& agent, float deltaTime)
{
	//deposit mushroom
	float distToTarget = X::Math::Magnitude(agent.position - agent.GetTargetDestination());
	if (distToTarget <= 10.0f)
	{

		agent.DepositMushrooms(agent.GetMushroomsCollected());
		agent.resetMushrooms();

		if (agent.GetMushroomsCollected() <= 0)
		{
			agent.setTimer(60.0f);
			agent.changeState(ravenStates::SearchForMushroom);

		}
	}

	
}

void GoHome::Exit(Raven& agent)
{
	

}

void GoHome::DebugUI()
{

}
void SearchForMushroom::Enter(Raven& agent)
{
	setPerception(agent.GetPerception());
}

void SearchForMushroom::Update(Raven& agent, float deltaTime)
{
	const auto& memoryRecords = mPerception->GetMemoryRecords();
	if (agent.getTimer() <= 0)
	{
		agent.changeState(ravenStates::GoHome);

	}
	
		for (auto& record : memoryRecords)
		{

			AgentType agentType = static_cast<AgentType>(record.GetProperty<int>("type", 0));
			if (agentType == AgentType::Mineral)
			{
				agent.changeState(ravenStates::MoveToMushroom);

			}
			if (agent.getTimer() <= 0.0f)
			{
				agent.changeState(ravenStates::GoHome);

			}
		}
	
		
	
   

}

void SearchForMushroom::Exit(Raven& agent)
{

}

void SearchForMushroom::DebugUI()
{

}
void SearchForMushroom::setPerception(const AI::PerceptionModule* eyes)
{
	mPerception = eyes;
}
void MoveToMushroom::Enter(Raven& agent)
{

}

void MoveToMushroom::Update(Raven& agent, float deltaTime)
{
	float distToTarget = X::Math::Magnitude(agent.position - agent.GetTargetDestination());
	if (distToTarget <= 10.0f)
	{
			agent.changeState(ravenStates::HarvestMushroom);
		
	}
}

void MoveToMushroom::Exit(Raven& agent)
{

}

void MoveToMushroom::DebugUI()
{

}

void HarvestMushroom::Enter(Raven& agent)
{

}

void HarvestMushroom::Update(Raven& agent, float deltaTime)
{
	if (agent.getHarvested() == true)
	{
		agent.setHarvested(false);
		agent.changeState(ravenStates::SearchForMushroom);
	}
	
	
}

void HarvestMushroom::Exit(Raven& agent)
{

}

void HarvestMushroom::DebugUI()
{

}


