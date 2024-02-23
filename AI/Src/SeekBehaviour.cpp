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

X::Math::Vector2 FleeBehaviour::Calculate(Agent& agent)
{
	X::Math::Vector2 desiredVelocity = X::Math::Vector2::Zero();
	const X::Math::Vector2 agentToDest = agent.destination - agent.position;

	if (X::Math::MagnitudeSqr(agentToDest) < panicDistance*panicDistance)
	{
		 desiredVelocity = -X::Math::Normalize(agentToDest) * agent.maxSpeed;

	}

	const X::Math::Vector2 fleeForce = desiredVelocity - agent.velocity;

	if (IsDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + desiredVelocity, X::Colors::Yellow);
		X::DrawScreenLine(agent.position, agent.position + agent.velocity, X::Colors::Green);
		X::DrawScreenCircle(agent.destination, 20.0f, X::Colors::Red);
		X::DrawScreenCircle(agent.destination, panicDistance, X::Colors::Pink);

	}
	return fleeForce;
}

X::Math::Vector2 ArriveBehaviour::Calculate(Agent& agent)
{
	const X::Math::Vector2 agentToDest = agent.destination - agent.position;
	const float distance = X::Math::Magnitude(agentToDest);

	if (X::Math::MagnitudeSqr(agentToDest) <= 1.0f)
	{
		return X::Math::Vector2::Zero();
	}

	const float decelTweaker = agent.mass * 1.5f;
	const float decelerationScale = (static_cast<float>(deceleration) + 1.0f)* decelTweaker;
	float speed = distance / decelerationScale;
	speed = X::Math::Min(speed, agent.maxSpeed);
	const X::Math::Vector2 desiredVelocity = (agentToDest / distance) * speed;
	const X::Math::Vector2 arriveForce = desiredVelocity - agent.velocity;

	if (IsDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + desiredVelocity, X::Colors::Yellow);
		X::DrawScreenLine(agent.position, agent.position + agent.velocity, X::Colors::Green);
		X::DrawScreenCircle(agent.destination, 20.0f, X::Colors::Red);
	}
	return arriveForce;
}
