#pragma once
#include "SteeringBehaviour.h"

namespace AI {
	class SteeringModule final {
	public:
		SteeringModule(Agent& agent);

		X::Math::Vector2 Calculate();
		template<class NewBehaviour>
		NewBehaviour* AddBehaviour() {
			static_assert(std::is_base_of_v<SteeringBehaviour, NewBehaviour),
				"SteeringModule: can only add type Steering Behaviour");
				auto& newBehaviour = mBehaviours.emplace_back(std::make_unique<NewBehaviour>());
				return static_cast<NewBehaviour*>(newBehaviour.get());

		}
	private:
		Agent& mAgent;
		std::vector<std::unique_ptr<SteeringBehaviour>> mBehaviours;
	};
}