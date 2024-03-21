#pragma once

#include "Goal.h"

namespace AI
{
	template<class AgentType>
	class GoalComposite : public Goal<AgentType>
	{
	public:
		using Status = Goal<AgentType>::Status;

		GoalComposite() = default;
		virtual ~GoalComposite() = default;

	protected:
		template<class GoalType>
		GoalType* AddSubGoal()
		{
			static_assert(std::is_base_of_v<Goal<AgentType>, GoalType>, "GoalComposite: subgoal must be of type Goal");
			auto& newGoal = mSubGoals.emplace_back(std::make_unique<GoalType>());
			return static_cast<GoalType*>(newGoal.get());
		}
		void RemoveAllSubGoals(AgentType& agent)
		{
			for (auto& goal : mSubGoals)
			{
				goal->Terminate(agent);
				goal.reset();
			}
			mSubGoals.clear();
		}

		Status ProcessSubGoals(AgentType& agent)
		{
			while (!mSubGoals.empty())
			{
				auto& goal = mSubGoals.back();
				if (goal->GetStatus() != Status::Completed &&
					goal->GetStatus() != Status::Failed)
				{
					break;
				}

				goal->Terminate(agent);
				goal.reset();
				mSubGoals.pop_back();
			}

			if (!mSubGoals.empty())
			{
				Status status = mSubGoals.back()->Process(agent);
				if (status == Status::Completed && mSubGoals.size() > 1)
				{
					return Status::Active;
				}

				return status;
			}

			return Status::Completed;
		}

		std::vector<std::unique_ptr<Goal<AgentType>>> mSubGoals;
	};
}