#pragma once

#include "SteeringBehaviour.h"

namespace AI
{
	class SeekBehaviour : public SteeringBehaviour
	{
	public:
		X::Math::Vector2 Calculate(Agent& agent) override;
	};
}