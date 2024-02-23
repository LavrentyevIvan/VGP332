#include "Precompiled.h"
#include "PursuitBehaviour.h"

#include "Agent.h"

using namespace AI;
X::Math::Vector2 PursuitBehaviour::Calculate(Agent& agent)
{
	if (agent.target == nullptr)
	{
		return X::Math::Vector2();
	}

	X::Math::Vector2 agentToTarget = agent.target->position - agent.position;
	float distToTarget = X::Math::Magnitude(agentToTarget);
	if (distToTarget <= 10.0f)
	{
		return X::Math::Vector2();
	}

	const float timeToTarget = distToTarget / agent.maxSpeed;
	const X::Math::Vector2& targetPosition = agent.target->position;
	const X::Math::Vector2& targetVelocity = agent.target->velocity;
	const X::Math::Vector2 predictedPosition = agent.target->position + targetVelocity * timeToTarget;

	agentToTarget = predictedPosition - agent.position;
	distToTarget = X::Math::Magnitude(agentToTarget);
	if (distToTarget <= 10.0f)
	{
		return X::Math::Vector2();
	}

	const X::Math::Vector2 desiredVelocity = (agentToTarget / distToTarget) * agent.maxSpeed;
	const X::Math::Vector2 pursuitForce = desiredVelocity - agent.velocity;

	if (IsDebug())
	{
		X::DrawScreenCircle(targetPosition, 20.0f, X::Colors::Red);
		X::DrawScreenLine(agent.position, agent.position + agent.velocity, X::Colors::Green);
		X::DrawScreenLine(agent.position, agent.position + desiredVelocity, X::Colors::Yellow);
	}

	return pursuitForce;
}