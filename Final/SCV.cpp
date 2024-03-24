#include "SCV.h"
#include "TypeIds.h"

#include "VisualSensor.h"

extern float viewRange;
extern float viewAngle;


namespace
{
	float ComputeImportance(const AI::Agent& agent, const AI::MemoryRecord& record) 
	{
		float score = 0.0f;
		AgentType entityType = static_cast<AgentType>(record.GetProperty<int>("type"));
		switch (entityType)
		{
		case AgentType::Invalid:
		{
			score = 0.0f;
			break;
		}
		case AgentType::SCV:
		{
			X::Math::Vector2 lastSeenPos = record.GetProperty<X::Math::Vector2>("lastSeenPosition");
			float distance = X::Math::Distance(agent.position, lastSeenPos);
			float distanceScore = std::max(500.0f - distance, 0.0f);
			score = distanceScore;
			break;
		}
		case AgentType::Mineral:
		{
			X::Math::Vector2 lastSeenPos = record.GetProperty<X::Math::Vector2>("lastSeenPosition");
			float distance = X::Math::Distance(agent.position, lastSeenPos);
			float distanceScore = std::max(1000.0f - distance, 0.0f);
			score = distanceScore;
			break;
		}
			default:
			break;

		}
		return 0.0f;

	}
}

SCV::SCV(AI::AIWorld& world)
	: Agent(world, static_cast<uint32_t>(AgentType::SCV))
{

}

void SCV::Load()
{
	mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, ComputeImportance);
	mPerceptionModule->SetMemorySpan(2.0f);
	mVisualSensor = mPerceptionModule->AddSensor<VisualSensor>();
	mVisualSensor->targetType = AgentType::Mineral;

	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	mSeekBehaviour = mSteeringModule->AddBehaviour<AI::SeekBehaviour>();
	mFleeBehaviour = mSteeringModule->AddBehaviour<AI::FleeBehaviour>();
	mArriveBehaviour = mSteeringModule->AddBehaviour<AI::ArriveBehaviour>();
	mWanderBehaviour = mSteeringModule->AddBehaviour<AI::WanderBehaviour>();
	mPursuitBehaviour = mSteeringModule->AddBehaviour<AI::PursuitBehaviour>();
	mSeperationBehaviour = mSteeringModule->AddBehaviour<AI::SeparationBehaviour>();
	mAlignmentBehaviour = mSteeringModule->AddBehaviour<AI::AlignmentBehaviour>();
	mCohesionBehaviour = mSteeringModule->AddBehaviour<AI::CohesionBehaviour>();

	

	const float screenWidth = static_cast<float>(X::GetScreenWidth());
	const float screenHeight = static_cast<float>(X::GetScreenHeight());
	destination = { screenWidth * 0.5f, screenHeight * 0.5f };

	for (int i = 0; i < mTextureIds.size(); ++i)
	{
		char name[128];
		sprintf_s(name, "scv_%02i.png", i + 1);
		mTextureIds[i] = X::LoadTexture(name);
	}
	
}

void SCV::Unload()
{
}

void SCV::Update(float deltatime)
{
	mVisualSensor->viewRange = viewRange;
	mVisualSensor->viewHalfAngle = viewAngle * X::Math::kDegToRad;


	mPerceptionModule->Update(deltatime);

	
	const X::Math::Vector2 force = mSteeringModule->Calculate();
	const X::Math::Vector2 acceleration = force / mass;
	velocity += acceleration * deltatime;

	position += velocity * deltatime;

	if (X::Math::MagnitudeSqr(velocity) > 1.0f)
	{
		heading = X::Math::Normalize(velocity);
	}

	const float screenWidth = static_cast<float>(X::GetScreenWidth());
	const float screenHeight = static_cast<float>(X::GetScreenHeight());
	if (position.x < 0.0f)
	{
		position.x += screenWidth;
	}
	if (position.x >= screenWidth)
	{
		position.x -= screenWidth;
	}
	if (position.y < 0.0f)
	{
		position.y += screenHeight;
	}
	if (position.y >= screenHeight)
	{
		position.y -= screenHeight;
	}

	const auto& memoryRecords = mPerceptionModule->GetMemoryRecords();
	for (auto& memory : memoryRecords)
	{
		X::Math::Vector2 pos = memory.GetProperty<X::Math::Vector2>("lastSeenPosition");
		X::DrawScreenLine(position, pos, X::Colors::White);

		std::string score = std::to_string(memory.importance);
		X::DrawScreenText(score.c_str(), pos.x, pos.y, 12.0f, X::Colors::White);
	}
}

void SCV::Render()
{
	const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
	const float percent = angle / X::Math::kTwoPi;
	const int frame = static_cast<int>(percent * mTextureIds.size()) % mTextureIds.size();
	X::DrawSprite(mTextureIds[frame], position);
}


void SCV::ShowDebug(bool debug)
{
	mSeekBehaviour->ShowDebug(debug);
	mFleeBehaviour->ShowDebug(debug);
	mArriveBehaviour->ShowDebug(debug);
	mWanderBehaviour->ShowDebug(debug);
	mPursuitBehaviour->ShowDebug(debug);
	mSeperationBehaviour->ShowDebug(debug);
	mAlignmentBehaviour->ShowDebug(debug);
	mCohesionBehaviour->ShowDebug(debug);
	

}

void SCV::SetSeek(bool active)
{
	mSeekBehaviour->SetActive(active);
}
void SCV::SetFlee(bool active)
{
	mFleeBehaviour->SetActive(active);

}
void SCV::SetArrive(bool active)
{
	mArriveBehaviour->SetActive(active);

}
void SCV::SetWander(bool active)
{
	mWanderBehaviour->SetActive(active);

}void SCV::SetPursuit(bool active)
{
	mPursuitBehaviour->SetActive(active);

}

void SCV::SetSeperation(bool active)
{
	mSeperationBehaviour->SetActive(active);
}

void SCV::SetAlignment(bool active)
{
	mAlignmentBehaviour->SetActive(active);
}

void SCV::SetCohesion(bool active)
{
	mCohesionBehaviour->SetActive(active);
}
void SCV::SetSeekWeight(float weight)
{
	mSeekBehaviour->SetWeight(weight);
}

void SCV::SetFleeWeight(float weight)
{
	mFleeBehaviour->SetWeight(weight);
}

void SCV::SetArriveWeight(float weight)
{
	mArriveBehaviour->SetWeight(weight);
}

void SCV::SetWanderWeight(float weight)
{
	mWanderBehaviour->SetWeight(weight);
}

void SCV::SetPursuitWeight(float weight)
{
	mPursuitBehaviour->SetWeight(weight);
}

void SCV::SetSeparationWeight(float weight)
{
	mSeperationBehaviour->SetWeight(weight);
}

void SCV::SetAlignmentWeight(float weight)
{
	mAlignmentBehaviour->SetWeight(weight);
}

void SCV::SetCohesionWeight(float weight)
{
	mCohesionBehaviour->SetWeight(weight);
}

