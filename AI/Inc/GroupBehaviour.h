#pragma once

#include "SteeringBehaviour.h"

namespace AI
{
	class SeparationBehaviour : public SteeringBehaviour
	{
	public:
		X::Math::Vector2 Calculate(Agent& agent) override;
	};

	class AlignmentBehaviour : public SteeringBehaviour
	{
	public:
		X::Math::Vector2 Calculate(Agent& agent) override;
	};

	class CohesionBehaviour : public SteeringBehaviour
	{
	public:
		X::Math::Vector2 Calculate(Agent& agent) override;
	};
}