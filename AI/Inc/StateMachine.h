#pragma once

#include "State.h"
namespace AI
{
	template<class AgentType>
	class StateMachine final {
	public:
		
		StateMachine() = default;
		
		void Initialize(AgentType* agent) {
			mAgent = agent;
		}
		void Terminate() {
			if (mCurrentState != nullptr) {
				mCurrentState->Exit(*mAgent);

			}
			mCurrentState = nullptr;
			mStates.clear();
			mAgent = nullptr;

		}
		template<class NewState>
		void AddState() {
			static_assert(std::is_base_of_v<State<AgentType>, NewState>, "StateMachine: can only add StateTypes");
			mStates.push_back(std::make_unique<NewState>());
		}
		
		void Update(float deltaTime) {
			if (mCurrentState != nullptr)
			{
				mCurrentState->Update(*mAgent, deltaTime);
			}
		}

		void ChangeState(int index) {
			if (mCurrentState != nullptr) 
			{
				mCurrentState->Exit(*mAgent);
			}
			mCurrentState = mStates[index].get();
			mCurrentState->Enter(*mAgent);
		}
		void DebugUI() {
			if (mCurrentState != nullptr) {
				mCurrentState->DebugUI;
			}
		}

	private:
		AgentType* mAgent = nullptr;
		State<AgentType>* mCurrentState = nullptr;
		std::vector<std::unique_ptr<State<AgentType>>> mStates;

	};
}

