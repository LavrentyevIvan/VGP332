#pragma once

#include<XEngine.h>

namespace AI {
	class Agent;
	class SteeringBehaviour {
	public:
		virtual ~SteeringBehaviour() = default;

		virtual X::Math::Vector2 Calculate(Agent& agent) = 0;

		void SetWeight(float weight) { mWeight = weight; }
		float GetWeight() const { return mWeight; }

		void SetActive(bool active) { mActive = active; }
		bool IsActive() const { return mActive; }

		void ShowDebug(bool debug) { mDebug = debug; }
		bool IsDebug() const { return mDebug; }


	private:
		float mWeight = 1.0f;
		float mActive = false;
		float mDebug = false;

	};
}