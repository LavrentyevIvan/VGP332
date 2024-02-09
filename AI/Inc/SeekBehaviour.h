#pragma once

#include "SteeringBehaviour.h"

namespace AI
{
	class SeekBehaviour : public SteeringBehaviour
	{
	public:
		X::Math::Vector2 Calculate(Agent& agent) override;
	};
	class FleeBehaviour : public SteeringBehaviour
	{
	public:
		X::Math::Vector2 Calculate(Agent& agent) override;

		float panicDistance = 100.0f;
	};
}