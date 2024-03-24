#include "RavenStates.h"
void GoHome::Enter(Raven& agent)
{
	agent.setTimer(10.0f);
}

void GoHome::Update(Raven& agent, float deltaTime)
{
	//deposit mushroom
	if (agent.GetTargetDestination().x == 120 && agent.GetTargetDestination().y == 120)
	{
		agent.DepositMushrooms(agent.GetMushroomsCollected());
	}
	if (agent.GetMushroomsCollected() == 0)
	{
		agent.setState(ravenStates::SearchForMushroom);

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
		agent.setState(ravenStates::GoHome);

	}
	else
	{
		for (auto& record : memoryRecords)
		{
			AgentType agentType = static_cast<AgentType>(record.GetProperty<int>("type", 0));
			if (agentType == AgentType::Mineral)
			{
				agent.setState(ravenStates::MoveToMushroom);

			}
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
	if (agent.position.x == agent.GetTargetDestination().x && agent.position.y == agent.GetTargetDestination().y)
	{
			agent.setState(ravenStates::HarvestMushroom);
		
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
		agent.setState(ravenStates::SearchForMushroom);
	}
	
	
}

void HarvestMushroom::Exit(Raven& agent)
{

}

void HarvestMushroom::DebugUI()
{

}


