#pragma once

#include "SteeringBehaviour.h"

namespace AI {
	class WanderBehaviour : public SteeringBehaviour {
	public:
		X::Math::Vector2 Calculate(Agent& agent) override;

		void Setup(float radius, float distance, float jitter);
		private:
			X::Math::Vector2 mLocalWanderTarget = X::Math::Vector2::Zero();
			float mWanderRadius = 1.0f;
			float mWanderDistance = 0.0f;
			float mWanderJitter = 0.1f;
	};
}