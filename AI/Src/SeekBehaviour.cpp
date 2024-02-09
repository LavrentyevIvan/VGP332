#include "Precompiled.h"
#include "SeekBehaviour.h"
#include "Agent.h"

using namespace AI;

X::Math::Vector2 SeekBehaviour::Calculate(Agent& agent)
{
	const X::Math::Vector2 agentToDest = agent.destination - agent.position;

	if (X::Math::MagnitudeSqr(agentToDest) <= 1.0f)
	{
		return X::Math::Vector2::Zero();
	}

	const X::Math::Vector2 desiredVelocity = X::Math::Normalize(agentToDest) * agent.maxSpeed;
	const X::Math::Vector2 seekForce = desiredVelocity - agent.velocity;

	if (IsDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + desiredVelocity, X::Colors::Yellow);
		X::DrawScreenLine(agent.position, agent.position + agent.velocity, X::Colors::Green);
		X::DrawScreenCircle(agent.destination, 20.0f, X::Colors::Red);
	}
	return seekForce;
}