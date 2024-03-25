#include <ImGui/Inc/imgui.h>
#include "Raven.h"

#include "TypeIds.h"
#include "VisualSensor.h"
#include "RavenStrategy.h"
#include "RavenHuntStrategy.h"
#include "RavenGoToMineralStrategy.h"
#include "RavenGoHomeStrategy.h"
#include "RavenHarvestStrategy.h"
#include "RavenStates.h"

extern float viewRange;
extern float viewAngle;
extern int homeStorage;

namespace
{
	float ComputeImportance(const AI::Agent& agent, const AI::MemoryRecord& record)
	{
		float score = 0.0f;
		AgentType entityType = static_cast<AgentType>(record.GetProperty<int>("type"));
		switch (entityType)
		{
		case AgentType::Invalid:
			score = 0.0f;
			break;
		case AgentType::SCV:
		{
			score = 0.0f;
		}
		break;
		case AgentType::Mineral:
		{
			int health = record.GetProperty<int>("health", 0);
			if (health > 0)
			{
				X::Math::Vector2 lastSeenPos = record.GetProperty<X::Math::Vector2>("lastSeenPosition");
				float distance = X::Math::Distance(agent.position, lastSeenPos);
				float distanceScore = std::max(1000.0f - distance, 0.0f);
				score = distanceScore;
			}
			else
			{
				score = 0.0f;
			}
		}
		break;
		default:
			break;
		}
		return score;
	}
}

Raven::Raven(AI::AIWorld& world)
	: Agent(world, static_cast<uint32_t>(AgentType::Raven))
{
}

void Raven::Load()
{

	mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, ComputeImportance);
	mPerceptionModule->SetMemorySpan(3.0f);
	mVisualSensor = mPerceptionModule->AddSensor<VisualSensor>();
	mVisualSensor->targetType = AgentType::Mineral;

	mDecisionModule = std::make_unique<AI::DecisionModule<Raven>>(*this);
	mDecisionModule->AddStrategy<RavenGoHomeStrategy>();
	mDecisionModule->AddStrategy<RavenHuntStrategy>();
	auto strategy = mDecisionModule->AddStrategy<RavenGoToMineralStrategy>();
	strategy->SetPerception(mPerceptionModule.get());
	mDecisionModule->AddStrategy<RavenHarvestStrategy>();

	rStateMachine.Initialize(this);
	rStateMachine.AddState<GoHome>();
	rStateMachine.AddState<SearchForMushroom>();
	rStateMachine.AddState<MoveToMushroom>();
	rStateMachine.AddState<HarvestMushroom>();

	changeState(ravenStates::SearchForMushroom);

	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	
	mSeekBehaviour = mSteeringModule->AddBehaviour<AI::SeekBehaviour>();
	mArriveBehaviour = mSteeringModule->AddBehaviour<AI::ArriveBehaviour>();

	

	for (int i = 0; i < mTextureIds.size(); ++i)
	{
		char name[128];
		sprintf_s(name, "interceptor_%02i.png", i + 1);
		mTextureIds[i] = X::LoadTexture(name);
	}


}

void Raven::Unload()
{
}

void Raven::Update(float deltaTime)
{

		
	
	

	mVisualSensor->viewRange = viewRange;
	mVisualSensor->viewHalfAngle = viewAngle * X::Math::kDegToRad;

	mPerceptionModule->Update(deltaTime);
	mDecisionModule->Update();
	rStateMachine.Update(deltaTime);

	//movement
	const X::Math::Vector2 force = mSteeringModule->Calculate();
	const X::Math::Vector2 acceleration = force / mass;
	velocity += acceleration * deltaTime;
	if (X::Math::MagnitudeSqr(velocity) > 1.0f)
	{
		heading = X::Math::Normalize(velocity);
	}

	timer-=1.0f*deltaTime;

	position += velocity * deltaTime;

	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	const auto& memoryRecords = mPerceptionModule->GetMemoryRecords();
	for (auto& memory : memoryRecords)
	{
		X::Math::Vector2 pos = memory.GetProperty<X::Math::Vector2>("lastSeenPosition");
		X::DrawScreenLine(position, pos, X::Colors::White);

		std::string score = std::to_string(memory.importance);
		X::DrawScreenText(score.c_str(), pos.x, pos.y, 12.0f, X::Colors::White);
	}
}

void Raven::Render()
{
	const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
	const float percent = angle / X::Math::kTwoPi;
	const int frame = static_cast<int>(percent * mTextureIds.size()) % mTextureIds.size();
	X::DrawSprite(mTextureIds[frame], position);
}

void Raven::ShowDebug(bool debug)
{
	mSeekBehaviour->ShowDebug(debug);

}

void Raven::SetSeek(bool active)
{
	mSeekBehaviour->SetActive(active);
}

void Raven::SetArrive(bool active)
{
	mArriveBehaviour->SetActive(active);
}


void Raven::SetTargetDestination(const X::Math::Vector2& targetDestination)
{
	mDestination = targetDestination;
}

X::Math::Vector2 Raven::GetTargetDestination()
{
	return mDestination;
}

void Raven::SetTarget(Entity* target)
{
	mTarget = target;
}

void Raven::CollectMushroom()
{
	collectedMushrooms += 1;
}


void Raven::changeState(ravenStates newState)
{
	rStateMachine.ChangeState((int)newState);
	currentState = newState;
}

int Raven::DepositMushrooms(int ravenMushrooms)
{
	homeStorage += ravenMushrooms;
	ravenMushrooms = 0;
	return ravenMushrooms;
}

void Raven::resetMushrooms()
{
	collectedMushrooms = 0;
}

void Raven::setHarvested(bool onHarvest)
{
	hasHarvested = onHarvest;
}

void Raven::setTimer(float count)
{
	timer = count;
}

