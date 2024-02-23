#include "Precompiled.h"
#include "GroupBehaviour.h"

#include "Agent.h"

using namespace AI;

X::Math::Vector2 AI::SeparationBehaviour::Calculate(Agent& agent)
{
	const float forceMultiplier = 5.0f;
	X::Math::Vector2 separationForce = X::Math::Vector2::Zero();
	for (auto& n : agent.neighbors)
	{
		if (n != agent.target)
		{
			X::Math::Vector2 dirToNeighbour = n->position - agent.position;
			float distance = X::Math::Magnitude(dirToNeighbour);
			float overlapDistance = (n->radius + agent.radius) - distance;
			if (overlapDistance > 0.0f)
			{
				dirToNeighbour /= distance;
			if (X::Math::Dot(dirToNeighbour, agent.heading) > 0.0f) 
				{
					float desiredSpeed = (overlapDistance / agent.radius) * forceMultiplier * agent.maxSpeed;
					desiredSpeed = X::Math::Min(desiredSpeed, agent.maxSpeed);
					separationForce += (-dirToNeighbour * desiredSpeed);
				}
			}
		}
	}

	if (X::Math::MagnitudeSqr(separationForce) > 1.0f)
	{
		separationForce = separationForce - agent.velocity;
	}

	if (IsDebug())
	{
		X::DrawScreenCircle(agent.position, agent.radius, X::Colors::Honeydew);
		X::DrawScreenLine(agent.position, agent.position + separationForce, X::Colors::Coral);

	}

	return separationForce;
}

X::Math::Vector2 AI::AlignmentBehaviour::Calculate(Agent& agent)
{
	X::Math::Vector2 alignmentForce;
	X::Math::Vector2 averageHeading;
	float totalAgents = 0;

	for (auto& n : agent.neighbors)
	{
		if (n != agent.target)
		{
			if (X::Math::Dot(agent.heading, n->heading) > 0.0f)
			{
				averageHeading += n->heading;
				++totalAgents;
			}
		}
	}

	if (totalAgents > 0.0f)
	{
		averageHeading /= totalAgents;
		alignmentForce = (averageHeading - agent.heading) * agent.maxSpeed;
	}

	if (IsDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + alignmentForce, X::Colors::Pink);
	}

	return alignmentForce;
}

X::Math::Vector2 AI::CohesionBehaviour::Calculate(Agent& agent)
{
	X::Math::Vector2 cohesionForce;
	X::Math::Vector2 centerOfMass;
	float totalAgents = 0.0f;

	for (auto& n : agent.neighbors)
	{
		if (n != agent.target)
		{
			centerOfMass += n->position;
			++totalAgents;
		}
	}
	if (totalAgents > 0.0f)
	{
		centerOfMass /= totalAgents;
		const X::Math::Vector2 desiredVelocity = X::Math::Normalize(centerOfMass - agent.position) * agent.maxSpeed;
		cohesionForce = desiredVelocity - agent.velocity;
	}

	if (IsDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + cohesionForce, X::Colors::Teal);
	}

	return cohesionForce;
}