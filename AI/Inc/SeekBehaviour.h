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
	class ArriveBehaviour : public SteeringBehaviour
	{
	public:
		enum class Deceleration {
			Fast,
			Normal,
			Slow
		};

		X::Math::Vector2 Calculate(Agent& agent) override;

		Deceleration deceleration = Deceleration::Normal;
	};
}